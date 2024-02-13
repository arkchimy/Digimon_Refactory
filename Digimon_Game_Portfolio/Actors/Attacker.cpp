#include "stdafx.h"
#include "Actors/Attacker.h"
#include "Sprite.h"
#include "Bullet.h"



D3DXVECTOR3 Init_Pos = { -275.0,-185,0 };
D3DXVECTOR3 LV1_Scale = { 100,100,1 };
D3DXVECTOR3 LV2_Scale = { 200,200,1 };


Attacker::Attacker(wstring imgfile, float width, float height, Sprites_Info info, int level)
{
	Init_info(info);
	CreateShaderAndBuffer(imgfile, width, height, info, level);

	CreateAnimation();
}

Attacker::Attacker(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
{
	Init_info(info);
	CreateShaderAndBuffer(imgfile, uvs, info, level);
	CreateAnimation();
}

Attacker::~Attacker()
{
	SAFE_DELETE(shader);

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

	cut_SceanFile = info.Skill_CutScene;

	buffer_vec.resize(Sprite_Type);
	buffer_vec[IDLE].reserve(info.idle_size);
	buffer_vec[Walk].reserve(info.Walk_size);
	buffer_vec[Action].reserve(info.Action_size);
	buffer_vec[Skill].reserve(info.Skill_size);
	buffer_vec[Hit].reserve(info.Hit_size);
	buffer_vec[Victory].reserve(info.victory_size);
	buffer_vec[Death].reserve(info.Death_size);
	buffer_vec[PowerUP].reserve(info.victory_size);
}

void Attacker::CreateShaderAndBuffer(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level)
{
	shader = new Shader(Texture_Shader);
	m_srv = Sprite_Manager::Load(imgfile);

	vector<vector<ComPtr<ID3D11Buffer>>> temp = Sprite_Manager::LoadBufferVector(imgfile);
	// buffer_vec 초기화에 실패한다면 
	if(temp.size() == 0 ) // Spirte에서 Buffer 와 Srv 초기화  
	{
		float start_w = 0.f, start_h = 0.f;

		for (int state_mode = 0; state_mode < Sprite_Type; state_mode++)
		{
			start_w = uvs[state_mode].x;
			start_h = uvs[state_mode].y;

			for (int i = 0; i < buffer_vec[state_mode].capacity(); i++)
			{
				shared_ptr<Sprite> temp = make_shared<Sprite>(buffer_vec[state_mode], imgfile, start_w, start_h, start_w + uvs[state_mode].z, uvs[state_mode].w);
				start_w += uvs[state_mode].z;
			}
		}
		// buffer store에 저장
		Sprite_Manager::StoreBufferVector(imgfile,buffer_vec);
	}
	else
		buffer_vec = temp;
	
	UpdateWorld();

}

void Attacker::CreateShaderAndBuffer(wstring imgfile, float width, float height, Sprites_Info info, int level)
{


	shader = new Shader(Texture_Shader);
	m_srv = Sprite_Manager::Load(imgfile);

	vector<vector<ComPtr<ID3D11Buffer>>> temp  = Sprite_Manager::LoadBufferVector(imgfile);
	if (temp.size() == 0) // Spirte에서 Buffer 와 Srv 초기화  
	{
		float start_w = 0.f, start_h = 0.f;
		for (int cnt = 0; cnt < Sprite_Type; cnt++)
		{
			start_w = 0.f;
			start_h = height * cnt;
			for (int i = 0; i < buffer_vec[cnt].capacity(); i++)
			{
				shared_ptr<Sprite> temp = make_shared<Sprite>(buffer_vec[cnt], imgfile, start_w, start_h, start_w + width, start_h + height);
				start_w += width;
			}
		}
		Sprite_Manager::StoreBufferVector(imgfile, buffer_vec);
	}
	else
		buffer_vec = temp;

	UpdateWorld();

}

void Attacker::CreateAnimation()
{
	animations.resize(Sprite_Type);

	animations[IDLE] = make_unique<Animation>(shader, m_srv, buffer_vec[IDLE], PlayMode::Loop);
	animations[Walk] = make_unique<Animation>(shader, m_srv, buffer_vec[Walk], PlayMode::Loop);
	animations[Action] = make_unique<Animation>(shader, m_srv, buffer_vec[Action], PlayMode::End);
	animations[Skill] = make_unique<Animation>(shader, m_srv, buffer_vec[Skill], PlayMode::End);
	animations[Hit] = make_unique<Animation>(shader, m_srv, buffer_vec[Hit], PlayMode::End);
	animations[Victory] = make_unique<Animation>(shader, m_srv, buffer_vec[Victory], PlayMode::Loop);
	animations[Death] = make_unique<Animation>(shader, m_srv, buffer_vec[Death], PlayMode::End_Stop);
	animations[PowerUP] = make_unique<Animation>(shader, m_srv, buffer_vec[Victory], PlayMode::End);


	for (int i = 0; i < animations.size(); i++)
		animations[i]->Set_Owner(this);

	Set_Mode(Walk);
}

void Attacker::UpdateSrvAndBuffer(wstring imgfile)
{
	m_srv = Sprite_Manager::Load(imgfile);
	// Store에 없는 경우를 주의해야함. Pooling 시에 미리 하나씩은 생성해두게 구현하자.
	buffer_vec = Sprite_Manager::LoadBufferVector(imgfile);

	CreateAnimation();
}


void Attacker::Update()
{
	animations[State]->Update();

}

void Attacker::Render()
{
	animations[State]->Render();
}

void Attacker::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

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

