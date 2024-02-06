#include "stdafx.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Animation.h"
extern float Sprite_Speed;

D3DXVECTOR3 Edit_Pos = { 30,30,0 };
D3DXVECTOR3 Edit_Pos2 = { 100,30,0 };
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

	team_id = ����;
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
	CheckTrue(buff_state >> Death); // ����

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
	// ���߰���
	//
	if (buff_state == Buff_State::Stun)
	{
		Stun_Time -= ImGui::GetIO().DeltaTime;
		//ȭ�� ������ ƨ����
		if (Position().y + Scale().y * 0.5f >= Height * 0.5f)
		{	// �������� �������� �����Ұ��
			D3DXVECTOR3	Screen_Normal = { 0,-1,0 };
			D3DXVECTOR3 Move_dir_Normal;
			Move_dir = -Move_dir;// ������ �ݴ����
			D3DXVec3Normalize(&Move_dir_Normal, &Move_dir);
			float n = D3DXVec3Dot(&Screen_Normal, &Move_dir);
			D3DXVECTOR3 Screen_vec = n * Screen_Normal;
			D3DXVECTOR3 A = Move_dir - Screen_vec;
			Move_dir = Move_dir - 2.f * A; //�ݻ����
		}
		else if (Position().y <= -245.f + Scale().y * 0.5f)
		{	// �Ʒ������� �������� �����Ұ��
			D3DXVECTOR3	Screen_Normal = { 0,1,0 };
			D3DXVECTOR3 Move_dir_Normal;
			Move_dir = -Move_dir;// ������ �ݴ����
			D3DXVec3Normalize(&Move_dir_Normal, &Move_dir);
			float n = D3DXVec3Dot(&Screen_Normal, &Move_dir);
			D3DXVECTOR3 Screen_vec = n * Screen_Normal;
			D3DXVECTOR3 A = Move_dir - Screen_vec;
			Move_dir = Move_dir - 2.f * A; //�ݻ����
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
	CheckFalse(State == Action); //Idle ���°� �ƴ϶�� ����

	D3DXVECTOR2 target_pos = { -Width,0 };

	int Bullet_Cnt = Normal_Bullet_Cnt;
	wstring Attack_Sprite_File;

	if (Skill_Type == �л���Ÿ��)
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
			bullets->Team_ID(����);
			//bullets->Position(sprites_vec[0][0]->Position());
			int dir = 0;
			if (i != 0)
				dir = 30 * i * (pow(-1, i));
			bullets->Fire({ target_pos.x, target_pos.y + dir });
		}
	}

}


