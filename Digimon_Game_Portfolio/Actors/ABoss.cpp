#include "stdafx.h"
#include "ABoss.h"
#include "Enemy.h"
#include "Animation.h"
#include "Sprite.h"
#include "Bullet.h"


ABoss::ABoss(wstring imgfile, float width, float height, Sprites_Info info, int level)
	:Enemy(imgfile, width, height, info, level)
{
	team_id = 적군;
	State = Walk;
	Move_dir = { -1,0,0 };
}

ABoss::ABoss(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
	:Enemy(imgfile,uvs,info,level)
{
	team_id = 적군;
	State = Walk;
	Move_dir = { -1,0,0 };
}

ABoss::~ABoss()
{
}

void ABoss::Take_Damage(Bullet* causer, D3DXVECTOR3 dir)
{
	
	UINT type = causer->Bullet_Type();
	Move_dir = dir;
	hp -= causer->Power();
	
	if (hp <= 0)
	{
		Set_Mode(Death);
		buff_state = Buff_State::Dead;
	}
	
}

void ABoss::Fire()
{
	CheckTrue(bBattle == 배틀종료);

	D3DXVECTOR2 target_pos;
	target_pos.x = Castle_Pos.x;
	target_pos.y = Castle_Pos.y;

	int Bullet_Cnt = Normal_Bullet_Cnt;
	wstring Attack_Sprite_File;

	if (Skill_Type == 분산형타입)
	{
		Bullet_Cnt = State == Action ? Normal_Bullet_Cnt : Skill_Bullet_Cnt; // Skill_Bullet_Cnt을 유닛마다 변경할 수 있게
	}
	Attack_Sprite_File = State == Action ? Normal_Sprite_File : Skill_Sprite_File;

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
			bullets->Fire({ Castle_Pos.x, Castle_Pos.y + dir });
			
		}
	}

	State = Walk;
}

bool ABoss::IsDeathMode()
{

	return State == Death;
}

void ABoss::Update()
{
	animations[State]->Update();
	CheckTrue(buff_state >> Death); // 죽음

	D3DXVECTOR3 current_Pos = Position();
	D3DXVECTOR3 dir;
	if (current_Pos.y < -245.f + Scale().y * 0.5f)
	{
		current_Pos.y = -245.f + Scale().y / 2.f;
	}

	dir = { current_Pos.x + speed.x * ImGui::GetIO().DeltaTime * Move_dir.x * knock_back, current_Pos.y + ImGui::GetIO().DeltaTime * Move_dir.y * speed.y, 0.f };

	if (bBattle)
		Position(dir);
	//
	// 폭발공격
	//
	
	if (distance >= current_Pos.x)
		bTurn = true;

	if (bTurn)
	{
		Move_dir = { 1,0,0 };
		Rotator({ 0,0,0 });
	}
	else Move_dir = { -1,0,0 };
	
	if (State == Walk)
		knock_back = 1.f;
	if (State == Action)
	{
		State = Action;
	
	}
	
}

void ABoss::Respawn()
{
	hp = 50.f;

	Set_Mode(Walk);

	bTurn = false;
	bBattle = true;
	buff_state = Buff_State::None;
	Move_dir = { -1,0,0 };
	Inter_Second = 2.f;
}
