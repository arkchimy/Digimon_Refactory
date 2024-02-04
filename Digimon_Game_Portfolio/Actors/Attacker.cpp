#include "stdafx.h"
#include "Actors/Attacker.h"
#include "Sprite.h"
#include "Bullet.h"
#include "Animation.h"
#include "Decal.h"

D3DXVECTOR3 Init_Pos = { -275.0,-185,0 };
D3DXVECTOR3 LV1_Scale = { 100,100,1 };
D3DXVECTOR3 LV2_Scale = { 200,200,1 };
D3DXVECTOR3 Decal_Circel_Scale = { 10,10,1.f };

Attacker::Attacker(wstring imgfile, float width, float height, Sprites_Info info, int level)
{
	init_info(imgfile,width,height,info,level);
	decal = make_unique<Decal>();
	cut_SceanFile = info.Skill_CutScene;
	
}

Attacker::Attacker(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info,  int level)
{
	init_info(imgfile, uvs, info, level);
	decal = make_unique<Decal>();
	cut_SceanFile = info.Skill_CutScene;
}

Attacker::~Attacker()
{
}

void Attacker::init_info(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
{
	Normal_Sprite_File = info.Normal_Sprite_File;
	Skill_Sprite_File = info.Skill_Sprite_File;
	Inter_Second = info.Fire_interval;
	Fire_idx = info.Fire_Timing_Idx;
	Skill_idx = info.Skill_Timing_Idx;

	Bullet_Type = info.Fire_Type;
	Skill_Type = info.Skill_Type;
	Skill_Percent = info.Skill_percentage;

	Normal_Bullet_Cnt = info.Bullet_Cnt;
	Skill_Bullet_Cnt = info.Skill_Bullet_Cnt;

	sprites_vec.resize(Sprite_Type);
	animations.resize(Sprite_Type);

	sprites_vec[IDLE].resize(info.idle_size);
	sprites_vec[Walk].resize(info.Walk_size);
	sprites_vec[Action].resize(info.Action_size);
	sprites_vec[Skill].resize(info.Skill_size);
	sprites_vec[Hit].resize(info.Hit_size);
	sprites_vec[Victory].resize(info.victory_size);
	sprites_vec[Death].resize(info.Death_size);
	sprites_vec[PowerUP].resize(info.victory_size);
	float start_w = 0.f, start_h = 0.f;

	for (int state_mode = 0; state_mode < Sprite_Type; state_mode++)
	{
		start_w = uvs[state_mode].x;
		start_h = uvs[state_mode].y;

		for (int i = 0; i < sprites_vec[state_mode].size(); i++)
		{
			sprites_vec[state_mode][i] = make_shared<Sprite>(imgfile, start_w, start_h, start_w + uvs[state_mode].z, uvs[state_mode].w);
			start_w += uvs[state_mode].z;
		}
	}
	
	if (level == 성장기)
	{
		for (int i = 0; i < Sprite_Type; i++)
		{
			for (int k = 0; k < sprites_vec[i].size(); k++)
			{
				sprites_vec[i][k]->Scale(LV1_Scale);
				sprites_vec[i][k]->Position(Init_Pos);

			}

		}
	}
	State = Walk;
	UpdateWorld();

	animations[IDLE] = make_unique<Animation>(sprites_vec[IDLE], PlayMode::Loop);
	animations[Walk] = make_unique<Animation>(sprites_vec[Walk], PlayMode::Loop);
	animations[Action] = make_unique<Animation>(sprites_vec[Action], PlayMode::End);
	animations[Skill] = make_unique<Animation>(sprites_vec[Skill], PlayMode::End);
	animations[Hit] = make_unique<Animation>(sprites_vec[Hit], PlayMode::End);
	animations[Victory] = make_unique<Animation>(sprites_vec[Victory], PlayMode::Loop);
	animations[Death] = make_unique<Animation>(sprites_vec[Death], PlayMode::End_Stop);
	animations[PowerUP] = make_unique<Animation>(sprites_vec[Victory], PlayMode::End);
	for (int i = 0; i < Sprite_Type; i++)
		animations[i]->Set_Owner(this);
}

void Attacker::init_info(wstring imgfile, float width, float height, Sprites_Info info, int level)
{

	Normal_Sprite_File = info.Normal_Sprite_File;
	Skill_Sprite_File = info.Skill_Sprite_File;
	Inter_Second = info.Fire_interval;
	Fire_idx = info.Fire_Timing_Idx;
	Skill_idx = info.Skill_Timing_Idx;

	Bullet_Type = info.Fire_Type;
	Skill_Type = info.Skill_Type;
	Skill_Percent = info.Skill_percentage;

	Skill_Bullet_Cnt = info.Bullet_Cnt;


	sprites_vec.resize(Sprite_Type);
	animations.resize(Sprite_Type);


	sprites_vec[IDLE].resize(info.idle_size);
	sprites_vec[Walk].resize(info.Walk_size);
	sprites_vec[Action].resize(info.Action_size);
	sprites_vec[Skill].resize(info.Skill_size);
	sprites_vec[Hit].resize(info.Hit_size);
	sprites_vec[Victory].resize(info.victory_size);
	sprites_vec[Death].resize(info.Death_size);
	sprites_vec[PowerUP].resize(info.victory_size);
	float start_w = 0.f, start_h = 0.f;

	for (int cnt = 0; cnt < Sprite_Type; cnt++)
	{
		start_w = 0.f;
		start_h = height * cnt;

		for (int i = 0; i < sprites_vec[cnt].size(); i++)
		{
			sprites_vec[cnt][i] = make_shared<Sprite>(imgfile, start_w, start_h, start_w + width, start_h + height);
			start_w += width;
		}
	}

	if (level == 성장기)
	{
		for (int i = 0; i < Sprite_Type; i++)
		{
			for (int k = 0; k < sprites_vec[i].size(); k++)
			{
				sprites_vec[i][k]->Scale(LV1_Scale);
				sprites_vec[i][k]->Position(Init_Pos);

			}

		}
	}
	else if (level == 보스)
	{
		for (int i = 0; i < Sprite_Type; i++)
		{
			for (int k = 0; k < sprites_vec[i].size(); k++)
			{
				sprites_vec[i][k]->Scale(LV2_Scale);
				sprites_vec[i][k]->Position(Init_Pos);

			}

		}
	}
	State = Walk;
	UpdateWorld();

	animations[IDLE] = make_unique<Animation>(sprites_vec[IDLE], PlayMode::Loop);
	animations[Walk] = make_unique<Animation>(sprites_vec[Walk], PlayMode::Loop);
	animations[Action] = make_unique<Animation>(sprites_vec[Action], PlayMode::End);
	animations[Skill] = make_unique<Animation>(sprites_vec[Skill], PlayMode::End);
	animations[Hit] = make_unique<Animation>(sprites_vec[Hit], PlayMode::End);
	animations[Victory] = make_unique<Animation>(sprites_vec[Victory], PlayMode::Loop);
	animations[Death] = make_unique<Animation>(sprites_vec[Death], PlayMode::End_Stop);
	animations[PowerUP] = make_unique<Animation>(sprites_vec[Victory], PlayMode::End);

	for (int i = 0; i < Sprite_Type; i++)
		animations[i]->Set_Owner(this);
}



void Attacker::Take_Damage( Bullet* causer,D3DXVECTOR3 dir)
{
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
		State = Death; //아직 Death Sprite를 안넣음
		buff_state = Buff_State::Dead;
		animations[State]->Start();
	}
	else
	{
		State = Hit;
		animations[State]->Start();
	}
}

void Attacker::Update()
{
	decal->Update();
	decal->Scale(Decal_Circel_Scale);
}

void Attacker::Render()
{
	CheckTrue(sprites_vec.size() == 0);
	decal->Render();
	animations[State]->Render();
	
}

void Attacker::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	CheckTrue(sprites_vec.size() == 0);
	for (int i = 0; i < Sprite_Type; i++)
	{
		for (int k = 0; k < sprites_vec[i].size(); k++)
			sprites_vec[i][k]->ViewProjection(V, P);
	}
	decal->ViewProjection(V, P);
}

void Attacker::UpdateWorld()
{
	for (int i = 0; i < Sprite_Type; i++)
	{
		for (int k = 0; k < sprites_vec[i].size(); k++)
			sprites_vec[i][k]->UpdateWorld();

	}
}



void Attacker::Position(D3DXVECTOR3 val)
{
	for (int i = 0; i < Sprite_Type; i++)
	{
		for (int k = 0; k < sprites_vec[i].size(); k++)
			sprites_vec[i][k]->Position(val);
	}
}

D3DXVECTOR3 Attacker::Position()
{
	return animations[State]->Position();
}

void Attacker::Scale(D3DXVECTOR3 val)
{
	for (int i = 0; i < Sprite_Type; i++)
	{
		for (int k = 0; k < sprites_vec[i].size(); k++)
			sprites_vec[i][k]->Scale(val);

	}
}

D3DXVECTOR3 Attacker::Scale()
{
	return animations[State]->Scale();
}

void Attacker::Rotator(D3DXVECTOR3 val)
{
	val.x = D3DXToRadian(val.x);
	val.y = D3DXToRadian(val.y);
	val.z = D3DXToRadian(val.z);
	for (int i = 0; i < Sprite_Type; i++)
	{
		for (int k = 0; k < sprites_vec[i].size(); k++)
			sprites_vec[i][k]->Rotator(val);

	}
}

D3DXVECTOR3 Attacker::Rotator()
{
	return sprites_vec[0][0]->Rotator();
}

shared_ptr<class Sprite> Attacker::Get_Sprite()
{
	int temp = floor(index);
	temp %= sprites_vec[State].size();

	return sprites_vec[State][temp];
}

bool Attacker::IsDeathMode()
{
	return State == Death;
}

void Attacker::Set_IdleMode()
{
	State = IDLE;  
	animations[State]->Start(); 
}

void Attacker::Set_StunMode()
{
	buff_state = Buff_State::Stun;
	Stun_Time = 1.f;
}

void Attacker::Decal_Visible(bool val)
{
	decal->Visible(val);
}
