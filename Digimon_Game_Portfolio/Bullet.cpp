#include "stdafx.h"
#include "Bullet.h"
#include "Sprite.h"
#include "Scene_Manager.h"
#include "Actors/Enemy.h"
#include "Animation.h"

Bullet::Bullet()
	:type(1), team_ID(0)
{
	Bullet(1);
}

Bullet::Bullet(UINT bullet_type)
	: type(bullet_type), team_ID(0)
{
	bvisible = false;
	m_Shader = new Shader(Texture_Shader);
}


Bullet::~Bullet()
{
	SAFE_DELETE(m_Shader);
}

void Bullet::Init_Info(Sprite_Info info, float bullet_speed, wstring effect_File, float knock_power)
{
	this->knock_back = knock_power;
	m_Speed = bullet_speed;

	current_idx = 0; // 표시할 idx
	m_SpriteSize = info.Sprite_cnt;

	Effect_File = effect_File;

	buffer_vec.reserve(m_SpriteSize);
	float start_w = 0, start_h = 0;
	int unit_w = info.Width;
	int unit_h = info.Height;

	m_srv = Sprite_Manager::Load(info.ImgFile);

	vector<ID3D11Buffer*> temp = Sprite_Manager::LoadBuffer(info.ImgFile);
	if (temp.size() == 0)
	{
		for (int i = 0; i < m_SpriteSize; i++)
		{
			make_shared<Sprite>(buffer_vec, info.ImgFile, start_w, start_h, start_w + unit_w, unit_h);
			start_w += unit_w;
		}
		Sprite_Manager::StoreBuffer(info.ImgFile, buffer_vec);
	}
	else
		buffer_vec = temp;



}
void Bullet::Init_Info(Sprite_Info info, float bullet_speed, vector<D3DXVECTOR4> pos_vec, wstring effect_File, float knock_power)
{
	this->knock_back = knock_power;
	m_Speed = bullet_speed;
	current_idx = 0;
	m_SpriteSize = info.Sprite_cnt;

	Effect_File = effect_File;


	buffer_vec.reserve(m_SpriteSize);
	m_srv = Sprite_Manager::Load(info.ImgFile);

	vector<ID3D11Buffer*> temp = Sprite_Manager::LoadBuffer(info.ImgFile);
	if (temp.size() == 0)
	{
		for (int i = 0; i < m_SpriteSize; i++)
		{
			make_shared<Sprite>(buffer_vec, info.ImgFile, pos_vec[i].x, pos_vec[i].y, pos_vec[i].z, pos_vec[i].w);
		}
		Sprite_Manager::StoreBuffer(info.ImgFile, buffer_vec);
	}
	else
		buffer_vec = temp;

}


void Bullet::CreateShaderAndBuffer()
{
}


void Bullet::Fire(D3DXVECTOR2 targetpos)
{
	explosion_Pos = { targetpos.x ,targetpos.y,0 };
	// 마우스 따라가는 Rotator
	{
		D3DXVECTOR3 current_pos = m_Position;
		D3DXVECTOR3 target_pos = { targetpos.x,targetpos.y,0 };
		m_Dir.x = targetpos.x - current_pos.x;
		m_Dir.y = targetpos.y - current_pos.y;
		m_Dir.z = 0;


		D3DXVec3Normalize(&target_pos, &target_pos);
		D3DXVec3Normalize(&m_Dir, &m_Dir);

		m_Dir.x *= m_Speed;
		m_Dir.y *= m_Speed;

		bvisible = true;

		// 중복 Hit 방지
		Targetted_Actor.clear();
	}

}

void Bullet::Update()
{
	CheckFalse(bvisible); //발사중 아니라면 리턴
	D3DXVECTOR3 current_Pos = m_Position;

	// 화면에 잡히는 범위를 벗어나면 visible 해제
	bool X_move = Width / 2.f <= current_Pos.x || current_Pos.x <= -Width / 2.f;
	bool Y_move = Height / 2.f <= current_Pos.y || current_Pos.y <= -Height / 2.f;

	if (X_move || Y_move)
	{
		bvisible = false;
		return;
	}
	m_Position = { current_Pos.x + m_Dir.x * m_Speed * ImGui::GetIO().DeltaTime, current_Pos.y + m_Dir.y * m_Speed * ImGui::GetIO().DeltaTime, 0 };
	UpdateWorld();
}

void Bullet::Render()
{
	CheckFalse(bvisible); //발사중 아니라면 리턴

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_localTime += ImGui::GetIO().DeltaTime * 9.f;
	current_idx = m_localTime;
	current_idx %= m_SpriteSize;

	m_Shader->AsShaderResource("Map")->SetResource(m_srv);

	DeviceContext->IASetVertexBuffers(0, 1, &buffer_vec[current_idx], &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader->Draw(0, 0, 6);

	if (team_ID == 0)
	{

		// 모든 적들과 위치 비교
		vector<shared_ptr<class Enemy>> enemies = Scene_Manager::Get_Enemies();
		if (type & UINT(Bullet_Type::explosion))
		{	// 밀치기 구현하기
			D3DXVECTOR3 target_vec = (explosion_Pos - m_Position);
			float distance = abs(D3DXVec3Length(&target_vec));
			if (distance <= 10.f) // 목표지점과 가까워지면
			{
				bvisible = false;
				shared_ptr<Animation> effect = Effect_Manager::Load(Effect_File);
				effect->Position(explosion_Pos);
				effect->Start();
				for (auto front : enemies)
				{
					if (front->IsDeathMode()) continue; // 죽어있는 상태라면 PASS
					D3DXVECTOR3 temp = front->Position();
					target_vec = front->Position() - explosion_Pos;
					distance = D3DXVec3Length(&target_vec);
					D3DXVECTOR3 dir;
					D3DXVec3Normalize(&dir, &target_vec);
					if (distance <= radius)
					{
						front->Set_StunMode();
						front->Take_Damage(this, dir);
					}
				}
			}
			return;
		}
		for (auto front : enemies)
		{
			if ((front->Buff() & Buff_State::Dead) == Buff_State::Dead)
				continue;
			if (Sprite::AABB(m_Position, front->Position()))
			{	// 총알 type에 따라  다른 이벤트 구현

				if (type & UINT(Bullet_Type::Normal))
				{	// 아무속성이 없다면 hit 시 사라짐
					bvisible = false;
					front->Take_Damage(this);
				}

				if (type & UINT(Bullet_Type::through))
				{	// 이미 타격된 적은 중복 타격 X
					auto iter = find(Targetted_Actor.begin(), Targetted_Actor.end(), front);
					if (iter == Targetted_Actor.end())
					{
						Targetted_Actor.push_back(front);
						front->Take_Damage(this);
					}
				}
				if (!(type & UINT(Bullet_Type::explosion)))
				{
					shared_ptr<Animation> effect = Effect_Manager::Load(Effect_File);
					effect->Position(m_Position);
					effect->Start();
				}
			}
		}


	}
	else if (team_ID == 적군)
	{
		D3DXVECTOR3 current_Pos = m_Position;
		if (current_Pos.x <= -440.f)
		{
			if (type & UINT(Bullet_Type::Normal))
			{	// 아무속성이 없다면 hit 시 사라짐
				bvisible = false;
				Scene_Manager::Take_Damage(this);
			}
			if (!(type & UINT(Bullet_Type::explosion)))
			{
				shared_ptr<Animation> effect = Effect_Manager::Load(Effect_File);
				effect->Position(m_Position);
				effect->Start();
			}
		}
	}
}

void Bullet::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotator.y, m_Rotator.x, m_Rotator.z);
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, m_Position.z);

	W = S * R * T;

	m_Shader->AsMatrix("World")->SetMatrix(W);
}

void Bullet::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	m_Shader->AsMatrix("View")->SetMatrix(V);
	m_Shader->AsMatrix("Projection")->SetMatrix(P);

}

void Bullet::Scale(D3DXVECTOR3 val)
{
	m_Scale = val;
	UpdateWorld();
}

void Bullet::Position(D3DXVECTOR3 val)
{
	m_Position = val;
	UpdateWorld();
}

void Bullet::Rotator(D3DXVECTOR3 val)
{
	val.x = D3DXToRadian(val.x);
	val.y = D3DXToRadian(val.y);
	val.z = D3DXToRadian(val.z);

	m_Rotator = val;
	UpdateWorld();
}

void Bullet::Init_BulletType(const UINT& type)
{
	this->type = type;
}



/// <summary>
///////////////////////////////////////////// Bullet_Manager
/// </summary>


vector<shared_ptr<Bullet>> Bullet_Manager::m;
int Bullet_Manager::idx;

void Bullet_Manager::Create(int poolsize)
{
	m.reserve(poolsize);
	for (int i = 0; i < poolsize; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::Normal));
		m.push_back(temp);
	}
}

shared_ptr<Bullet> Bullet_Manager::Load(wstring imgfile)
{

	idx++;
	idx %= m.size();
	if (imgfile.compare(Digimon_Folder + L"길몬_브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"길몬_브레스.png",//File
					2,									//Sprite_Cnt 
					25,									// Width
					25									// Height
			},
			35.f,										//Speed
			Effect_Folder + L"Hit_0.png"
		);
		m[idx]->Init_BulletType(UINT(Bullet_Type::Normal));
		m[idx]->Scale({ 30,30,1.f });
	}
	if (imgfile.compare(Digimon_Folder + L"길몬_스킬브레스.png") == 0)
	{

		m[idx]->Init_Info(
			Sprite_Info
			{
				Digimon_Folder + L"길몬_스킬브레스.png",  //File
				7,									      //Sprite_Cnt 
				0,
				0
			},
			25.f,  //Speed							
			{
			D3DXVECTOR4{0,0,44,23},
			D3DXVECTOR4{80,0,124,23},
			D3DXVECTOR4{155,0,200,23},
			D3DXVECTOR4{243,0,284,23},
			D3DXVECTOR4{319,0,364,23},
			D3DXVECTOR4{399,0,446,23},
			D3DXVECTOR4{482,0,525,24},
			},
			Effect_Folder + L"Explosion_0.png"
			);
		m[idx]->Init_BulletType(UINT(Bullet_Type::explosion));
		m[idx]->Scale({ 60,30,1.f });
	}
	if (imgfile.compare(Digimon_Folder + L"레나몬_브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
				Digimon_Folder + L"레나몬_브레스.png",  //File
				1,									//Sprite_Cnt 
				53,									// Width
				19									// Height
			},

			30.f,		//Speed	

			Effect_Folder + L"Hit_0.png"
		);
		m[idx]->Init_BulletType(UINT(Bullet_Type::Normal));
		m[idx]->Scale({ 53,19,1.f });
	}
	if (imgfile.compare(Digimon_Folder + L"레나몬_스킬브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"레나몬_스킬브레스.png",//File
					1,									//Sprite_Cnt 
					0,									// Width
					0									// Height
			},
			15.f,  //Speed	
			{
			D3DXVECTOR4{0,0,7,6},
			},
			Effect_Folder + L"Hit_0.png"
			);
		m[idx]->Init_BulletType(UINT(Bullet_Type::Normal));
		m[idx]->Scale({ 10,10,1.f });
	}

	if (imgfile.compare(Digimon_Folder + L"테리어몬_브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"테리어몬_브레스.png",//File
					6,									//Sprite_Cnt 
					0,									// Width
					0									// Height
			},
			35.f,		//Speed	
			{
			D3DXVECTOR4{0,2,3,5},
			D3DXVECTOR4{5,2,10,5},
			D3DXVECTOR4{12,0,20,7},
			D3DXVECTOR4{22,0,30,7},
			D3DXVECTOR4{35,1,44,6},
			D3DXVECTOR4{50,1,59,6},
			},
			Effect_Folder + L"Hit_0.png"
			);
		m[idx]->Scale({ 20,20,1.f });
		m[idx]->Init_BulletType(UINT(Bullet_Type::Normal));
	}
	if (imgfile.compare(Digimon_Folder + L"테리어몬_스킬브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"테리어몬_스킬브레스.png",//File
					5,									//Sprite_Cnt 
					0,									// Width
					0									// Height
			},
			15.f,		//Speed	
			{
			D3DXVECTOR4{0,0,19,22},
			D3DXVECTOR4{24,0,46,22},
			D3DXVECTOR4{52,0,72,22},
			D3DXVECTOR4{79,0,98,22},
			D3DXVECTOR4{105,0,125,22},
			},
			Effect_Folder + L"Hit_0.png"
			);
		m[idx]->Scale({ 50,50,1.f });
		m[idx]->Init_BulletType(UINT(Bullet_Type::through));
	}
	if (imgfile.compare(Digimon_Folder + L"가르고몬_브레스.png") == 0)
	{
		m[idx]->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"가르고몬_브레스.png",//File
					1,									//Sprite_Cnt 
					8,									// Width
					8									// Height
			},
			35.f,		//Speed	

			Effect_Folder + L"Hit_0.png"
		);
		m[idx]->Init_BulletType(UINT(Bullet_Type::Normal));
		m[idx]->Scale({ 8,8,1.f });
	}
	
	return m[idx];
}

void Bullet_Manager::Render()
{
	for (int i = 0; i < BulletPool; i++)
		m[i]->Render();
}

void Bullet_Manager::Update()
{
	for (int i = 0; i < BulletPool; i++)
		m[i]->Update();
}

void Bullet_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < BulletPool; i++)
		m[i]->ViewProjection(V, P);
}

/*
*  Effect_Class
*/
vector<Sprite_Info> effectfile
{	// 이펙트 루트 지정
	{
		Effect_Folder + L"Hit_0.png",
		5,
		192.f,
		192.f,
	},
	{
		Effect_Folder + L"Explosion_0.png",
		5,
		25.f,
		25.f,
	},
	// 레벨업은 따로 해야함.
	{
		Effect_Folder + L"Level_Up.png",
		4,
		16.f,
		16.f,
	},
};
vector<Shader*> Effect_Manager::shader_vec;
vector<ID3D11ShaderResourceView*> Effect_Manager::srv_vec;
vector<vector<ID3D11Buffer*>> Effect_Manager::buffer_vec;

vector<shared_ptr<Animation>> Effect_Manager::animations;
int Effect_Manager::shader_idx = 0;

/// /////////////////
// 이펙트가 추가될 경우 effectfile 파일에 정보를 추가할 것
/// /////////////////
void Effect_Manager::Create(int idx)
{
	float start_w, start_h, width, height;
	int effect_Size = effectfile.size();

	srv_vec.resize(effect_Size);
	buffer_vec.resize(effect_Size);


	for (int k = 0; k < effect_Size; k++)
	{
		start_w = 0;
		start_h = 0;
		width = effectfile[k].Width;
		height = effectfile[k].Height;

		srv_vec[k] = Sprite_Manager::Load(effectfile[k].ImgFile);
		buffer_vec[k] = Sprite_Manager::LoadBuffer(effectfile[k].ImgFile);
		// buffer_vec 가  LoadBuffer에 실패하여 초기화가 안됬다면
		if (buffer_vec[k].size() == 0)
		{
			for (int i = 0; i < effectfile[k].Sprite_cnt; i++)
			{
				//Sprite 생성자 인자 고치기
				make_shared<Sprite>(buffer_vec[k], effectfile[k].ImgFile, start_w, start_h, start_w + width, start_h + height, Effect_Shader);
				start_w += width;
			}
			// buffer 정보 저장
			Sprite_Manager::StoreBuffer(effectfile[k].ImgFile, buffer_vec[k]);
		}

	}
	shader_vec.reserve(Poolsize);
	animations.reserve(Poolsize);

	// shader 당 animation하다 담당
	for (int i = 0; i < Poolsize; i++)
	{
		shader_vec.emplace_back(new Shader(Effect_Shader));//14ms , 10ms
		animations.emplace_back(make_shared<Animation>(shader_vec[i], srv_vec[i % effect_Size], buffer_vec[i % effect_Size], PlayMode::End));
		animations[i]->Scale({ 150,150,1 });
	}


}

shared_ptr<Animation> Effect_Manager::Load(wstring imgfile)
{

	for (int i = 0; i < effectfile.size(); i++)
	{
		if (effectfile[i].ImgFile.compare(imgfile) == 0)
		{
			// 범위 초과 방지
			shader_idx++;
			shader_idx %= shader_vec.size();

			// srv 와 buffer 업데이트
			animations[shader_idx]->UpdateSrvAndBuffer(srv_vec[i], buffer_vec[i]);
			return animations[shader_idx];
		}
	}
	cout << "애니메이션 Compare 실패" << endl;
	return animations[0];
}

void Effect_Manager::Render()
{
	//애니메이션에서 render하기
	for (auto animation : animations)
		animation->Render();

}

void Effect_Manager::Update()
{
	for (auto animation : animations)
		animation->Update();

}

void Effect_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (Shader* shader : shader_vec)
	{
		shader->AsMatrix("View")->SetMatrix(V);
		shader->AsMatrix("Projection")->SetMatrix(P);
	}

	//level_up->ViewProjection(V, P);
}

void Effect_Manager::Level_Up(D3DXVECTOR3 pos)
{
	cout << "아직 미구현" << endl;
	/*level_up->Position(pos);
	level_up->Start();*/
}

