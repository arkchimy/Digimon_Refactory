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
	Init_info(info);
	CreateShaderAndBuffer(imgfile,width,height,info,level);

	
}

Attacker::Attacker(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info,  int level)
{
	Init_info(info);
	CreateShaderAndBuffer(imgfile, uvs, info, level);

}

Attacker::~Attacker()
{
}

void Attacker::CreateShaderAndBuffer(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
{
	shader = new Shader(Texture_Shader);
	
	// Spirte에서 Buffer 와 Srv 초기화  
	{
		float start_w = 0.f, start_h = 0.f;

		for (int state_mode = 0; state_mode < Sprite_Type; state_mode++)
		{
			start_w = uvs[state_mode].x;
			start_h = uvs[state_mode].y;

			for (int i = 0; i < srv_vec[state_mode].capacity(); i++)
			{
				shared_ptr<Sprite> temp = make_shared<Sprite>(srv_vec[state_mode], buffer_vec[state_mode], imgfile, start_w, start_h, start_w + uvs[state_mode].z, uvs[state_mode].w);
				start_w += uvs[state_mode].z;
			}
		}
	}

	State = Walk;

	UpdateWorld();


}

void Attacker::Init_info(const Sprites_Info& info)
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

	decal = make_unique<Decal>();
	cut_SceanFile = info.Skill_CutScene;

	srv_vec.resize(Sprite_Type);
	buffer_vec.resize(Sprite_Type);

	srv_vec[IDLE].reserve(info.idle_size);
	srv_vec[Walk].reserve(info.Walk_size);
	srv_vec[Action].reserve(info.Action_size);
	srv_vec[Skill].reserve(info.Skill_size);
	srv_vec[Hit].reserve(info.Hit_size);
	srv_vec[Victory].reserve(info.victory_size);
	srv_vec[Death].reserve(info.Death_size);
	srv_vec[PowerUP].reserve(info.victory_size);

}

void Attacker::CreateShaderAndBuffer(wstring imgfile, float width, float height, Sprites_Info info, int level)
{


	shader = new Shader(Texture_Shader);

	float start_w = 0.f, start_h = 0.f;
	for (int cnt = 0; cnt < Sprite_Type; cnt++)
	{
		start_w = 0.f;
		start_h = height * cnt;
		for (int i = 0; i < srv_vec[cnt].capacity(); i++)
		{
			shared_ptr<Sprite> temp = make_shared<Sprite>(srv_vec[cnt], buffer_vec[cnt], imgfile, start_w, start_h, start_w + width, start_h + height);
			start_w += width;
		}
	}

	State = Walk;
	UpdateWorld();

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
		//animations[State]->Start();
	}
	else
	{
		State = Hit;
		//animations[State]->Start();
	}
}

void Attacker::Update()
{
	decal->Update();
	decal->Scale(Decal_Circel_Scale);
}

void Attacker::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	indexing += ImGui::GetIO().DeltaTime * Sprite_Speed;
	idx = indexing;
	idx %= srv_vec[State].size();

	shader->AsShaderResource("Map")->SetResource(srv_vec[State][idx]);

	DeviceContext->IASetVertexBuffers(0, 1, &buffer_vec[State][idx], &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
}

void Attacker::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	decal->ViewProjection(V, P);
}

void Attacker::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotator.y, rotator.x, rotator.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	W = S * R * T;
	world = W; // world  업데이트

	shader->AsMatrix("World")->SetMatrix(W);
}

void Attacker::Position(D3DXVECTOR3 val)
{
	position = val;
	UpdateWorld();
}

void Attacker::Scale(D3DXVECTOR3 val)
{
	scale = val;
	UpdateWorld();
}

void Attacker::Rotator(D3DXVECTOR3 val)
{
	val.x = D3DXToRadian(val.x);
	val.y = D3DXToRadian(val.y);
	val.z = D3DXToRadian(val.z);

	rotator = val;
	UpdateWorld();
}

bool Attacker::IsDeathMode()
{
	return State == Death;
}

void Attacker::Set_IdleMode()
{
	State = IDLE;  
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