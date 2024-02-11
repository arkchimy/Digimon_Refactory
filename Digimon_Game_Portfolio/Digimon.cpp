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
	animations[State]->Update();
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
			Set_Mode(IDLE);
			return;
		}
		Position({ current.x + dir.x * move_speed * ImGui::GetIO().DeltaTime, current.y + dir.y * move_speed * ImGui::GetIO().DeltaTime, 0 });
		
		return;
	}
	decal->Update();

	FindLookAtTarget();

	if (State == IDLE)
	{
		Fire_time += ImGui::GetIO().DeltaTime;
		if (Fire_time >= Inter_Second)
		{
			Fire_time = 0.f;

			if (bBattle == 배틀시작)
			{
				
				unsigned int    number;

				rand_s(&number);
				number = (unsigned int)((double)number /
					((double)UINT_MAX + 1) * 100.0);
				number < Skill_Percent ? Set_Mode(Skill) : Set_Mode(Action);
			}
		}
	}
	else if (State == Action)
	{
		idx = animations[State]->Current_Idx();
		if (idx == Fire_idx)
			Fire();
	}
	else if (State == Skill)
	{
		idx = animations[State]->Current_Idx();
		if (idx == Skill_idx)
			Fire();
	}
}


void Digimon::Take_Damage(Bullet* causer, D3DXVECTOR3 dir)
{
	//Attacker::Take_Damage(causer, dir);
	cout << "아군 디지몬 타격" << endl; //아직 기획안함
}

void Digimon::FindLookAtTarget()
{
	// 매 프레임마다 가장 가까운적 찾기
	target = SearchTarget();
	
	if (target == nullptr)
	{   // 적이 없을 때  전투 중일떄와 클리어할떄를 구분지어야 할듯. Victory 와 Idle 결정 요소
		Rotator({ 0,0,0 });
		Fire_time = 0.f;
		return;
	}
	else
	{
		D3DXVECTOR3 TargetPos = target->Position();
		Skill_Target_Pos.x = TargetPos.x;
		Skill_Target_Pos.y = TargetPos.y;


		D3DXVECTOR3 direction = TargetPos - Position();
		// Rotation에서 다시 라디안으로 바꿈
		bullet_Degree = D3DXToDegree(atan2(direction.y, direction.x));

		bullet_Dir = direction;
	}

}

shared_ptr<Enemy> Digimon::SearchTarget()
{
	shared_ptr<Enemy> Target = nullptr;
	D3DXVECTOR3 Pos = Position();
	D3DXVECTOR3 Target_Pos = { 0,0,0 };
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
	CheckTrue(bMove); // 움직이는 중이라면 idle로 안바꿈
	Set_Mode(IDLE);
}

void Digimon::Stage_Clear()
{
	Set_Mode(Victory);
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
				int dir = 0;

				if (State == Skill)
				{
					bullets->Fire({ Skill_Target_Pos.x, Skill_Target_Pos.y + dir });
				}

			}
		}
	}
	else
	{
		// 샷건형 총알의 발사간격
		float bullet_distance = 0.f;
		for (int i = 0; i < Bullet_Cnt; i++)
		{
			shared_ptr<Bullet> bullets = Bullet_Manager::Load(Attack_Sprite_File);
			//Spread Type   => Double Shot   Triple Shot
			{
				bullets->Team_ID(아군);
				bullets->Position(position);

				int dir = 0;
				if (i != 0)
					dir = bullet_distance * (pow(-1, i));
				if (i % 2 == 0)
					bullet_distance += 30.f;
				if (State == Skill)
				{
					bullets->Fire({ Skill_Target_Pos.x, Skill_Target_Pos.y + dir });
				}

				D3DXVec3Normalize(&bullet_Dir, &bullet_Dir);
				bullets->Set_Dir({ bullet_Dir.x,bullet_Dir.y + 0.1f * dir / 30.f ,bullet_Dir.z });
				bullets->Rotator({ 0,0, bullet_Degree });

			}
		}
	}
	Set_Mode(IDLE);
}

void Digimon::Animation_Start(int idx)
{
	Set_Mode(Skill);
	Skill_Cut->Cut_Start(cut_SceanFile);

}

void Digimon::MoveTo(D3DXVECTOR3 goal)
{
	Set_Mode(Walk);
	goal_pos = goal;
	bMove = true;
}

bool Digimon::ClickEvent(D3DXVECTOR2 mouse)
{
	D3DXVECTOR3 pos = Position();
	before_pos = pos;
	D3DXVECTOR3 scale = { 30,30,0 };
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


	//같은 유닛인지 체크
	if (drag_digimon->cut_SceanFile.compare(cut_SceanFile) != 0)
		return false;
	D3DXVECTOR3 pos = Position();
	D3DXVECTOR3 scale = { 30,30,0 };
	bool x_chk = pos.x - scale.x <= mouse.x && pos.x + scale.x >= mouse.x;
	bool y_chk = pos.y - scale.y <= mouse.y && pos.y + scale.y >= mouse.y;

	
	if (x_chk && y_chk)
	{
		if (Inter_Second > 0.5f)
			Inter_Second -= 0.5f;
		
		Effect_Manager::Level_Up(pos);
		Set_Mode(PowerUP);

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
		return Digimon_Manager::Load(next_digimon);
		//return Card_Manager::FindDigimon(next_digimon);
	}
	return nullptr;
}


/// <summary>
/// Digimon_Manager class
/// </summary>

#define Digimon_Type 4 // 길몬, 레나몬 , 테리어몬 , 가르고몬

vector<shared_ptr<Digimon>> Digimon_Manager::m;
int Digimon_Manager::idx = -1;
void Digimon_Manager::CreatePooling(int pool)
{
	// 풀링 만큼 Digimon을 만들었다는데 의의가 있음.
	// 나중에 Load하면서 srv와 buffer는 새로 업데이트가능.
	// m에서 순차적으로
	for (int i = 0; i < pool; i++)
	{
		int ID = i % Digimon_Type;
		switch (ID)
		{
		case 0:
			m.push_back(
				make_shared<Digimon>(Digimon_Folder + L"길몬.png", 40.f, 40.f, Guilmon, 성장기)
			);
			break;
		case 1:
			m.emplace_back(
				make_shared<Digimon>(Digimon_Folder + L"레나몬.png", Renamon_UV,
					Renamon,
					성장기));
			break;
		case 2:
			m.emplace_back(
				make_shared<Digimon>(Digimon_Folder + L"테리어몬.png", Terriermon_UV,
					Terriermon,
					성장기));
			break;
		case 3:
			m.emplace_back(
				make_shared<Digimon>(Digimon_Folder + L"가르고몬.png", Galgomon_UV,
					Galgomon,
					성장기));
			break;
		default:
			break;
		}

	}
}

void Digimon_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (auto digimon : m)
		digimon->ViewProjection(V, P);
}


shared_ptr<Digimon> Digimon_Manager::Load(wstring imgfile)
{
	Sprites_Info info;
	if (imgfile.compare(Digimon_Folder + L"가르고몬.png") == 0)
		info = Galgomon;
	else if (imgfile.compare(Digimon_Folder + L"레나몬.png") == 0)
		info = Renamon;
	else if (imgfile.compare(Digimon_Folder + L"길몬.png") == 0)
		info = Guilmon;
	else if (imgfile.compare(Digimon_Folder + L"테리어몬.png") == 0)
		info = Terriermon;

	idx++;
	idx %= m.size();

	m[idx]->Init_info(info);
	m[idx]->UpdateSrvAndBuffer(imgfile);

	return m[idx];
}
