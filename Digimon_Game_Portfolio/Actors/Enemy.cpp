#include "stdafx.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Animation.h"
extern float Sprite_Speed;

D3DXVECTOR3 Edit_Pos = { 30,30,0 };
D3DXVECTOR3 Edit_Pos2 = { 80,30,0 };
D3DXVECTOR3 Edit_Scale = { 10,10,1.f };


Enemy::Enemy(wstring imgfile, float width, float height, Sprites_Info info, int level)
	:Attacker(imgfile, width, height, info, level)
{
	Init_info();
}

Enemy::Enemy(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
	: Attacker(imgfile, uvs, info, level)
{
	Init_info();
}


void Enemy::Init_info()
{

	team_id = 적군;
	Set_Mode(Walk);
	Move_dir = { -1,0,0 };

}

Enemy::~Enemy()
{

}

void Enemy::Take_Damage(Bullet* causer, D3DXVECTOR3 dir)
{
	//Attacker::Take_Damage(causer,dir);
	UINT type = causer->Bullet_Type();
	Move_dir = dir;
	hp -= causer->Power();
	if (type & UINT(Bullet_Type::Normal))
	{
		knock_back = causer->Knock_Back();
	}
	if (type & UINT(Bullet_Type::explosion))
	{
		Move_dir = dir * 5.f;
	}
	if (hp <= 0)
	{
		buff_state = Buff_State::Dead;
		Set_Mode(Death);
	}
	else
	{
		Set_Mode(Hit);
	}
}

void Enemy::Update()
{
	animations[State]->Update();
	CheckTrue(buff_state >> Death); // 죽음

	D3DXVECTOR3 current_Pos = Position();
	D3DXVECTOR3 temp_scale = Scale();
	D3DXVECTOR3 dir;

	if (current_Pos.y < -245.f + temp_scale.y * 0.5f)
	{
		current_Pos.y = -245.f + temp_scale.y / 2.f;
	}
	if (current_Pos.y >= Width / 2.f - temp_scale.y / 2.f)
	{
		current_Pos.y = Width / 2.f - temp_scale.y / 2.f - 2e-3f;
	}

	dir = { current_Pos.x + Edit_Pos2.x * ImGui::GetIO().DeltaTime * Move_dir.x * knock_back, current_Pos.y + ImGui::GetIO().DeltaTime * Move_dir.y * Edit_Pos2.y, 0.f };

	if (bBattle)
		Position(dir);
	//
	// 폭발공격
	//
	if (buff_state == Buff_State::Stun)
	{
		Stun_Time -= ImGui::GetIO().DeltaTime;
		//화면 밖으로 튕길경우
		if (Position().y + Scale().y * 0.5f >= Height * 0.5f)
		{	// 위쪽으로 끝지점에 도달할경우
			D3DXVECTOR3	Screen_Normal = { 0,-1,0 };
			D3DXVECTOR3 Move_dir_Normal;
			Move_dir = -Move_dir;// 벡터의 반대방향
			D3DXVec3Normalize(&Move_dir_Normal, &Move_dir);
			float n = D3DXVec3Dot(&Screen_Normal, &Move_dir);
			D3DXVECTOR3 Screen_vec = n * Screen_Normal;
			D3DXVECTOR3 A = Move_dir - Screen_vec;
			Move_dir = Move_dir - 2.f * A; //반사백터
		}
		else if (Position().y <= -245.f + Scale().y * 0.5f)
		{	// 아래쪽으로 끝지점에 도달할경우
			D3DXVECTOR3	Screen_Normal = { 0,1,0 };
			D3DXVECTOR3 Move_dir_Normal;
			Move_dir = -Move_dir;// 벡터의 반대방향
			D3DXVec3Normalize(&Move_dir_Normal, &Move_dir);
			float n = D3DXVec3Dot(&Screen_Normal, &Move_dir);
			D3DXVECTOR3 Screen_vec = n * Screen_Normal;
			D3DXVECTOR3 A = Move_dir - Screen_vec;
			Move_dir = Move_dir - 2.f * A; //반사백터
		}
		if (Stun_Time < 0.f)
		{
			buff_state = None;
			Stun_Time = 0.f;
			Set_Mode(Walk);

			if (bTurn) Move_dir = { 1,0,0 };
			else Move_dir = { -1,0,0 };
		}

	}
	else
	{
		if (distance >= current_Pos.x)
			bTurn = true;

		if (bTurn)
		{
			Move_dir = { 1,0,0 };
			Rotator({ 0,0,0 });
		}
		else
		{
			Move_dir = { -1,0,0 };
			Rotator({ 0,180,0 });
		}
	}
	if (State == Walk)
		knock_back = 1.f;
	if (State == Action)
	{
		if (idx == Fire_idx)
			Fire();
	}
}

void Enemy::Render()
{
	animations[State]->Render();

}

void Enemy::Battle(bool val)
{
	bBattle = val;
	Set_Mode(Walk);
}

void Enemy::Stage_Clear()
{

}

void Enemy::Respawn()
{
	hp = 3.f;

	bTurn = false;
	bBattle = true;
	buff_state = Buff_State::None;
	Move_dir = { -1,0,0 };

	Set_Mode(Walk);
}

void Enemy::Fire()
{
	CheckFalse(State == Action); //Idle 상태가 아니라면 리턴

	D3DXVECTOR2 target_pos = { -Width,0 };

	int Bullet_Cnt = Normal_Bullet_Cnt;
	wstring Attack_Sprite_File;

	if (Skill_Type == 분산형타입)
	{
		Bullet_Cnt = State == Action ? Normal_Bullet_Cnt : Skill_Bullet_Cnt;
	}
	Attack_Sprite_File = State == Action ? Normal_Sprite_File : Skill_Sprite_File;


	Set_Mode(Walk);

	for (int i = 0; i < Bullet_Cnt; i++)
	{
		shared_ptr<Bullet> bullets = Bullet_Manager::Load(Attack_Sprite_File);
		//Spread Type   => Double Shot   Triple Shot
		{
			bullets->Team_ID(적군);
			//bullets->Position(sprites_vec[0][0]->Position());
			int dir = 0;
			if (i != 0)
				dir = 30 * i * (pow(-1, i));
			bullets->Fire({ target_pos.x, target_pos.y + dir });
		}
	}

}


#define Digimon_Type 4 // 길몬, 레나몬 , 테리어몬 , 가르고몬

vector<shared_ptr<Enemy>> Enemy_Manager::m;
int Enemy_Manager::idx = -1;
void Enemy_Manager::CreatePooling(int pool)
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
				make_shared<Enemy>(Digimon_Folder + L"길몬.png", 40.f, 40.f, Guilmon, 성장기)
			);
			break;
		case 1:
			m.emplace_back(
				make_shared<Enemy>(Digimon_Folder + L"레나몬.png", Renamon_UV,
					Renamon,
					성장기));
			break;
		case 2:
			m.emplace_back(
				make_shared<Enemy>(Digimon_Folder + L"테리어몬.png", Terriermon_UV,
					Terriermon,
					성장기));
			break;
		case 3:
			m.emplace_back(
				make_shared<Enemy>(Digimon_Folder + L"가르고몬.png", Galgomon_UV,
					Galgomon,
					성장기));
			break;
		default:
			break;
		}

	}
}

void Enemy_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (auto digimon : m)
		digimon->ViewProjection(V, P);
}


shared_ptr<Enemy> Enemy_Manager::Load(wstring imgfile)
{
	idx++;
	idx %= m.size();

	m[idx]->UpdateSrvAndBuffer(imgfile);

	return m[idx];
}
