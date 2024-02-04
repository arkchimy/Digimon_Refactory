#include "stdafx.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Animation.h"
extern float Sprite_Speed;

D3DXVECTOR3 Edit_Pos = { 30,30,0 };
D3DXVECTOR3 Edit_Pos2 = {100,30,0 };
D3DXVECTOR3 Edit_Scale = { 10,10,1.f };


Enemy::Enemy(wstring imgfile, float width, float height, Sprites_Info info, int level)
	:Attacker(imgfile, width, height, info, level)
{
	team_id = 적군;
	State = Walk;
	Move_dir = { -1,0,0 };
}

Enemy::Enemy(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
	:Attacker(imgfile,uvs,info,level)
{
	team_id = 적군;
	State = Walk;
	Move_dir = { -1,0,0 };
}

Enemy::~Enemy()
{

}

void Enemy::Take_Damage(Bullet* causer,D3DXVECTOR3 dir)
{
	Attacker::Take_Damage(causer,dir);
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
		current_Pos.y = -245.f + temp_scale.y / 2.f ;
	}
	if (current_Pos.y >= Width / 2.f - temp_scale.y / 2.f)
	{
		current_Pos.y = Width / 2.f - temp_scale.y / 2.f - 2e-3f;
	}

	dir = { current_Pos.x + Edit_Pos2.x * Time::Delta() * Move_dir.x * knock_back, current_Pos.y + Time::Delta() * Move_dir.y * Edit_Pos2.y, 0.f };
	
	if(bBattle)
		Position(dir);
	//
	// 폭발공격
	//
	if (buff_state == Buff_State::Stun) 
	{
		Stun_Time -= Time::Delta();
		//화면 밖으로 튕길경우
		if (Position().y + Scale().y * 0.5f >= Height * 0.5f)
		{	// 위쪽으로 끝지점에 도달할경우
			D3DXVECTOR3	Screen_Normal = { 0,-1,0 };
			D3DXVECTOR3 Move_dir_Normal;
			Move_dir = -Move_dir;// 벡터의 반대방향
			D3DXVec3Normalize(&Move_dir_Normal,&Move_dir);
			float n = D3DXVec3Dot(&Screen_Normal, &Move_dir);
			D3DXVECTOR3 Screen_vec = n * Screen_Normal;
			D3DXVECTOR3 A = Move_dir - Screen_vec;
			Move_dir = Move_dir - 2.f * A; //반사백터
		}
		else if (Position().y  <= -245.f  + Scale().y * 0.5f)
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
			State = Walk;
			if(bTurn) Move_dir = { 1,0,0 };
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
		else Move_dir = { -1,0,0 };
	}
	if (State == Walk)
		knock_back = 1.f;
	if (State == Action)
	{
		if (animations[State]->Current_Idx() == Fire_idx)
			Fire();
	}
}

void Enemy::Battle(bool val)
{
	bBattle = val;
}

void Enemy::Stage_Clear()
{

}

void Enemy::Respawn()
{
	hp = 3.f;
	State = Walk;
	bTurn = false;
	bBattle = true;
	buff_state = Buff_State::None;
	Move_dir = { -1,0,0 };
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


	State = Walk;
	
	for (int i = 0; i < Bullet_Cnt; i++)
	{
		shared_ptr<Bullet> bullets = Bullet_Manager::Load(Attack_Sprite_File);
		//Spread Type   => Double Shot   Triple Shot
		{
			bullets->Team_ID(적군);
			bullets->Position(sprites_vec[0][0]->Position());
			int dir = 0;
			if (i != 0)
				dir = 30 * i * (pow(-1, i));
			bullets->Fire({ target_pos.x, target_pos.y + dir });
		}
	}

}
