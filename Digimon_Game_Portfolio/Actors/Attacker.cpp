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
	/*Normal_Sprite_File = info.Normal_Sprite_File;
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

	Digimon_Sprite::Load(imgfile, width, height, sprites_vec);


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
		======================================================================*/
	
	
	
	
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

	shader = new Shader(Texture_Shader);
	float start_w2 = 0.f, start_h2 = 0.f;
	for (int i = 0; i < sprites_vec[0].size(); i++)
	{

		shared_ptr<Sprite> temp = make_shared<Sprite>(srv_vec,buffer_vec, imgfile, start_w2, start_h2, start_w2 + width, start_h2 + height);
		start_w2 += width;
		//CreateBufferAndInit(imgfile, start_w2, start_h2, start_w2 + width, start_h2 + height);
	}
	

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
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	indexing += ImGui::GetIO().DeltaTime;
	int idx = indexing;
	idx %= srv_vec.size();

	shader->AsShaderResource("Map")->SetResource(srv_vec[idx]);


	DeviceContext->IASetVertexBuffers(0, 1, &buffer_vec[idx], &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);

	CheckTrue(sprites_vec.size() == 0);
	decal->Render();
	animations[State]->Render();
	
}

void Attacker::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

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
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotator.y, rotator.x, rotator.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	W = S * R * T;
	//W = S * T;
	world = W; // world  업데이트
	shader->AsMatrix("World")->SetMatrix(W);

	if (rotator.y >= 3.14f)
		W = S * T;

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

void Attacker::CreateBufferAndInit(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y)
{
	D3DXMatrixIdentity(&world);// world 초기화

	D3DXVECTOR3 TextSize;
	D3DX11_IMAGE_INFO info;
	D3DX11GetImageInfoFromFile(imgFile.c_str(), NULL, &info, NULL);

	TextSize.x = float(info.Width);
	TextSize.y = float(info.Height);
	TextSize.z = 1.f;

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);


	start_X = (start_X > 0) ? start_X / TextSize.x : 0.f;
	start_Y = (start_Y > 0) ? start_Y / TextSize.y : 0.f;
	End_X = (End_X > 1.f) ? End_X / TextSize.x : 1.f;
	End_Y = (End_Y > 1.f) ? End_Y / TextSize.y : 1.f;

	vertices[0].Uv = D3DXVECTOR2(start_X, End_Y);
	vertices[1].Uv = D3DXVECTOR2(start_X, start_Y);
	vertices[2].Uv = D3DXVECTOR2(End_X, End_Y);
	vertices[3].Uv = D3DXVECTOR2(End_X, End_Y);
	vertices[4].Uv = D3DXVECTOR2(start_X, start_Y);
	vertices[5].Uv = D3DXVECTOR2(End_X, start_Y);


	float sizeX = (End_X > 0) ? End_X * (float)info.Width : (float)info.Width;
	sizeX -= start_X * (float)info.Width;

	float sizeY = (End_Y > 0) ? End_Y * (float)info.Height : (float)info.Height;
	sizeY -= start_Y * (float)info.Height;

	TextSize = D3DXVECTOR3(sizeX, sizeY, 1);

	scale.x *= TextSize.x;
	scale.y *= TextSize.y;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &buffer_vec[srv_vec.size()-1]);
		assert(SUCCEEDED(hr));
	}
}



map<wstring, vector<vector<shared_ptr<class Sprite>>>> Digimon_Sprite::m;

void Digimon_Sprite::Load(wstring imgfile, float width, float height, vector<vector<shared_ptr<class Sprite>>>& output)
{
	auto iter = m.find(imgfile);
	// 못찾았다면
	if (iter == m.end())
	{
		vector<vector<shared_ptr<class Sprite>>> temp;

		temp.resize(output.size());

		float start_w = 0.f, start_h = 0.f;

		for (int cnt = 0; cnt < Sprite_Type; cnt++)
		{
			start_w = 0.f;
			start_h = height * cnt;
			temp[cnt].resize(output[cnt].size());

			for (int i = 0; i < temp[cnt].size(); i++)
			{
				temp[cnt][i] = make_shared<Sprite>(imgfile, start_w, start_h, start_w + width, start_h + height);
				start_w += width;
			}
		}
		m[imgfile] = temp;
	}
	output = m[imgfile];
}


