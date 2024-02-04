#include "stdafx.h"
#include "Digimon.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Scene_Manager.h"
#include "Actors/Enemy.h"
#include "Animation.h"
#include "Decal.h"
#include "CutManager.h"
#include "Reward_Card.h"

extern float Sprite_Speed;
extern bool NumKey_1;
float move_speed = 200.f;

Digimon::Digimon(wstring imgfile, float width, float height, Sprites_Info info, int level)
	:Attacker(imgfile, width, height, info, level)
{
	next_digimon = info.Next_digimon;
	team_id = 아군;
}

Digimon::Digimon(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
	: Attacker(imgfile, uvs, info, level)
{
	next_digimon = info.Next_digimon;
	team_id = 아군;
}


Digimon::~Digimon()
{

}




void Digimon::Update()
{
	if (bDrag)
	{
		D3DXVECTOR2 hit_point;
		hit_point.x = Mouse_Pos.x - Width / 2.f;
		hit_point.y = -(Mouse_Pos.y - Height / 2.f);
		Position({ hit_point.x,hit_point.y,0 });
		return;
	}
	if (bMove)
	{
		State = Walk;
		D3DXVECTOR3 current = Position();
		D3DXVECTOR3 vec = goal_pos - current;
		D3DXVECTOR3 dir = goal_pos - current;
		D3DXVec3Normalize(&dir, &dir);
		float distance = abs(D3DXVec3Length(&vec));
		if (distance < 10.f)
		{
			Position(goal_pos);
			Rotator({ 0,0,0 });
			bMove = false;
			State = IDLE;
			animations[State]->Start();
			
			return;
		}
		Position({ current.x + dir.x * move_speed * Time::Delta(), current.y + dir.y * move_speed * Time::Delta(), 0 });
		animations[State]->Update();
		return;
	}
	animations[State]->Update();
	decal->Update();

	FindLookAtTarget(); 

	if (State == IDLE)
	{
		Fire_time += Time::Delta();
		if (Fire_time >= Inter_Second)
		{
			Fire_time = 0.f;
			index = 0; //  상태 전환으로 인한 인덱스 초기화
			if (bBattle == 배틀시작)
			{
				State = Action;
				animations[State]->Start();
			}
		}
	}
	else if (State == Action)
	{
		if (animations[State]->Current_Idx() == Fire_idx)
			Fire();
	}
	else if (State == Skill)
	{
		if (animations[State]->Current_Idx() == Skill_idx)
			Fire();
	}
}


void Digimon::Take_Damage(Bullet* causer, D3DXVECTOR3 dir)
{
	Attacker::Take_Damage(causer, dir);
}

void Digimon::FindLookAtTarget()
{
	CheckTrue(R_Push); // 마우스가 눌린 상태면 PASS함
	shared_ptr<Enemy> target = SearchTarget();
	if (target == nullptr) 
	{   // 적이 없을 때  전투 중일떄와 클리어할떄를 구분지어야 할듯. Victory 와 Idle 결정 요소
		Rotator({ 0,0,0 });
		Fire_time = 0.f;
		return;
	}
	else
	{
		D3DXVECTOR3 direction = target->Position() - Position();
		float degree = D3DXToDegree(atan(direction.y, direction.x));
		Rotator({ 0, 0, degree });

		bullet_Dir = direction;
	}

}

shared_ptr<Enemy> Digimon::SearchTarget()
{
	shared_ptr<Enemy> Target = nullptr;
	D3DXVECTOR3 Pos = Position();
	D3DXVECTOR3 Target_Pos = {0,0,0};
	float distance = 750.f;

	vector<shared_ptr<Enemy>> enemies = Scene_Manager::Get_Enemies();
	for (auto front : enemies)
	{
		if (front->IsDeathMode())
			continue;
		//enemy가 살아있는지 체크해야함.
		D3DXVECTOR3 enemy_pos = front->Position();
		D3DXVECTOR3 temp = Pos - enemy_pos;

		float distance_temp;
		distance_temp = abs(D3DXVec3Length(&temp));
		//distance_temp = sqrt(pow(temp.x, 2) + pow(temp.y, 2)); // 피타고라스 
		if (distance > distance_temp)
		{	// 더 가까운 적을 발견했다면
			distance = distance_temp;
			Target = front;
		}
		
	}
	return Target;
}


void Digimon::Battle(bool val)
{
	bBattle = val;
	CheckFalse(val);
	State = IDLE;
}

void Digimon::Stage_Clear()
{
	State = Victory;
	animations[State]->Start();
}

void Digimon::Fire()
{
	CheckTrue(bBattle == 배틀종료);
	CheckTrue(State == IDLE); //Idle 상태가 아니라면 리턴

	D3DXVECTOR2 target_pos;
	float aspect = (float)Width / Height;
	target_pos.x = Mouse_Pos.x - Width / 2.f;
	target_pos.y = -(Mouse_Pos.y + Height / -2.f);

	int Bullet_Cnt = Normal_Bullet_Cnt;
	wstring Attack_Sprite_File;

	if (Skill_Type == 분산형타입) 
	{
		Bullet_Cnt = State == Action ? Normal_Bullet_Cnt : Skill_Bullet_Cnt; // Skill_Bullet_Cnt을 유닛마다 변경할 수 있게
	}
	
	Attack_Sprite_File = State == Action ? Normal_Sprite_File : Skill_Sprite_File;

	if (Skill_Type == 연사형타입)
	{
		for (int i = 0; i < Bullet_Cnt; i++)
		{
			shared_ptr<Bullet> bullets = Bullet_Manager::Load(Attack_Sprite_File);
			//Spread Type   => Double Shot   Triple Shot
			{
				bullets->Team_ID(아군);
				bullets->Position(sprites_vec[0][0]->Position());

				int dir = 0;

				if (State == Skill)
				{
					target_pos.x = Skill_Target_Pos.x - Width / 2.f;
					target_pos.y = -(Skill_Target_Pos.y + Height / -2.f);

					bullets->Fire({ target_pos.x, target_pos.y + dir });
				}
				
			}
		}
	}
	else 
	{
		for (int i = 0; i < Bullet_Cnt; i++)
		{
			shared_ptr<Bullet> bullets = Bullet_Manager::Load(Attack_Sprite_File);
			//Spread Type   => Double Shot   Triple Shot
			{
				bullets->Team_ID(아군);
				bullets->Position(sprites_vec[0][0]->Position());

				int dir = 0;
				if (i != 0)
					dir = 30 * i * (pow(-1, i));
				if (State == Skill)
				{
					target_pos.x = Skill_Target_Pos.x - Width / 2.f;
					target_pos.y = -(Skill_Target_Pos.y + Height / -2.f);

					bullets->Fire({ target_pos.x, target_pos.y + dir });
				}
				else if (!R_Push)
				{	//마우스가 안 눌린상태
					D3DXVec3Normalize(&bullet_Dir, &bullet_Dir);
					bullets->Set_Dir({ bullet_Dir.x,bullet_Dir.y + 0.1f * dir / 30.f ,bullet_Dir.z });
					bullets->Rotator(Rotator());
				}


			}
		}
	}
	
	State = IDLE;
}

void Digimon::Animation_Start(int idx)
{
	animations[idx]->Start();
	Skill_Cut->Cut_Start(cut_SceanFile);
	
}

void Digimon::MoveTo(D3DXVECTOR3 goal)
{
	State = Walk;
	animations[State]->Start();
	goal_pos = goal;
	bMove = true;
}

bool Digimon::ClickEvent(D3DXVECTOR2 mouse)
{
	D3DXVECTOR3 pos = Position();
	before_pos = pos;
	D3DXVECTOR3 scale = Scale();
	bool x_chk = pos.x - scale.x <= mouse.x && pos.x + scale.x >= mouse.x;
	bool y_chk = pos.y - scale.y <= mouse.y && pos.y + scale.y >= mouse.y;
	if (x_chk && y_chk)
	{
		bDrag = true;
		return true;
	}
	return false;
}

bool Digimon::ClickEvent(D3DXVECTOR2 mouse, shared_ptr<class Digimon> drag_digimon)
{
	if (bDrag) return false; // 드래그 중 인 대상 PASS

	if (drag_digimon->cut_SceanFile.compare(cut_SceanFile) != 0)
		return false;
	D3DXVECTOR3 pos = Position();
	D3DXVECTOR3 scale = Scale();
	bool x_chk = pos.x - scale.x <= mouse.x && pos.x + scale.x >= mouse.x;
	bool y_chk = pos.y - scale.y <= mouse.y && pos.y + scale.y >= mouse.y;

	//같은 레벨인지도 체크하기
	if (x_chk && y_chk) 
	{
		animations[Action]->PlaySpeed(4.0f);
		if(Inter_Second > 0.5f)
			Inter_Second -= 0.5f;
		animations[Skill]->PlaySpeed(4.0f);
		Effect_Manager::Level_Up(pos);
		State = PowerUP;
		animations[State]->Start();
		combine_cnt++;
		return true;
	}

	return false;
}

void Digimon::Drag(bool val)
{
	bDrag = val; 
	Position(before_pos);
}

shared_ptr<class Digimon> Digimon::Evolution()
{
	if (combine_cnt >= 2) // 2번쨰 합침이면
	{
		combine_cnt = 0;
		return Card_Manager::FindDigimon(next_digimon);
	}
	return nullptr;
}
