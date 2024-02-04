#include "stdafx.h"
#include "Bullet.h"
#include "Sprite.h"
#include "Scene_Manager.h"
#include "Actors/Enemy.h"
#include "Animation.h"

Bullet::Bullet()
	:type(1), team_ID(0)
{
	bvisible = false;
}

Bullet::Bullet(UINT bullet_type)
	: type(bullet_type), team_ID(0)
{
	bvisible = false;
}


Bullet::~Bullet()
{
}

void Bullet::Init_Info(wstring ImgFile, float cnt,float bullet_speed,float start)
{
	speed = bullet_speed;
	current_idx = 0;
	idx_size = cnt;
	sprites.resize(idx_size);
	float start_w =0, start_h = start;
	int unit = 80;

	
	for (int i = 0; i < idx_size; i++)
	{
		sprites[i] = make_shared<Sprite>(ImgFile, start_w, start_h, start_w + unit, start_h + unit);
		start_w += unit;
	}
	
}
void Bullet::Init_Info(Sprite_Info info, float bullet_speed, wstring effect_File, float knock_power)
{
	this->knock_back = knock_power;
	speed = bullet_speed;
	current_idx = 0;
	idx_size = info.Sprite_cnt;
	sprites.resize(idx_size);
	Effect_File = effect_File;

	float start_w = 0, start_h = 0;
	int unit_w = info.Width;
	int unit_h = info.Height;


	for (int i = 0; i < idx_size; i++)
	{
		sprites[i] = make_shared<Sprite>(info.ImgFile, start_w, start_h, start_w + unit_w, unit_h);
		start_w += unit_w;
	}



}
void Bullet::Init_Info(Sprite_Info info, float bullet_speed, vector<D3DXVECTOR4> pos_vec, wstring effect_File, float knock_power)
{
	this->knock_back = knock_power;
	speed = bullet_speed;
	current_idx = 0;
	sprites.resize(info.Sprite_cnt);
	Effect_File = effect_File;
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i] = make_shared<Sprite>(info.ImgFile, pos_vec[i].x, pos_vec[i].y, pos_vec[i].z, pos_vec[i].w);
	}
}
void Bullet::Init_Info(wstring ImgFile,  float cnt,  float bullet_speed, float width, float height, float knock_power)
{
	this->knock_back = knock_power;
	speed = bullet_speed;
	current_idx = 0;
	idx_size = cnt;
	sprites.resize(idx_size);
	float start_w = 0, start_h = 0;
	int unit_w = width;
	int unit_h = height;


	for (int i = 0; i < idx_size; i++)
	{
		sprites[i] = make_shared<Sprite>(ImgFile, start_w, start_h, start_w + unit_w, unit_h);
		start_w += unit_w;
	}

}
void Bullet::Init_Info(wstring ImgFile, float cnt, float bullet_speed,vector<D3DXVECTOR4> pos_vec ,float knock_power)
{
	this->knock_back = knock_power;
	speed = bullet_speed;
	current_idx = 0;
	idx_size = cnt;
	sprites.resize(idx_size);
	
	for (int i = 0; i < idx_size; i++)
	{
		sprites[i] = make_shared<Sprite>(ImgFile, pos_vec[i].x, pos_vec[i].y, pos_vec[i].z, pos_vec[i].w);
	}

}


void Bullet::Fire(D3DXVECTOR2 targetpos)
{
	explosion_Pos = { targetpos.x ,targetpos.y,0};
	// 마우스 따라가는 Rotator
		{
			D3DXVECTOR3 current_pos = sprites[current_idx]->Position();
			D3DXVECTOR3 target_pos = { targetpos.x,targetpos.y,0 };
			Dir.x = targetpos.x - current_pos.x;
			Dir.y = targetpos.y - current_pos.y;
			Dir.z = 0;

			D3DXVec3Normalize(&target_pos, &target_pos);


			float result = atan2(Dir.y, Dir.x);
			//result = D3DXToRadian(result);
			Rotator({ 0,0,result });
			D3DXVec3Normalize(&Dir, &Dir);

			Dir.x *= speed;
			Dir.y *= speed;

			bvisible = true;
			Targetted_Actor.clear();
		}

}

void Bullet::Update()
{
	CheckFalse(bvisible); //발사중 아니라면 리턴
	D3DXVECTOR3 current_Pos = sprites[current_idx]->Position();

	bool X_move = Width / 2.f <= current_Pos.x - sprites[current_idx]->Scale().x/2.f || current_Pos.x <= -Width/2.f;
	bool Y_move = Height / 2.f <= current_Pos.y || current_Pos.y <= -Height / 2.f;

	if (X_move || Y_move) 
	{
		bvisible = false;
		return;
	}
	//Position({ current_Pos.x + speed *  Time::Delta() , current_Pos.y,0 });
	Position({ current_Pos.x + Dir.x * speed  * Time::Delta() , current_Pos.y + Dir.y * speed * Time::Delta(),0 });
}

void Bullet::Render()
{
	CheckFalse(bvisible); //발사중 아니라면 리턴
	sprites[current_idx]->Render();
	current_idx += speed * 30 * Time::Delta();
	current_idx %= sprites.size();
	
	
	if (team_ID == 0)
	{
		vector<shared_ptr<class Enemy>> enemies = Scene_Manager::Get_Enemies();
		if (type & UINT(Bullet_Type::explosion))
		{	// 밀치기 구현하기
			D3DXVECTOR3 target_vec = (explosion_Pos - sprites[current_idx]->Position());
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
			if (Sprite::OBB(sprites[current_idx], front->Get_Sprite()))
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
					effect->Position(sprites[current_idx]->Position());
					effect->Start();
				}
			}
		}
		
		
	}
	else if (team_ID == 적군)
	{
		D3DXVECTOR3 current_Pos = sprites[current_idx]->Position();
		if (current_Pos.x  <= -440.f )
		{
			if (type & UINT(Bullet_Type::Normal))
			{	// 아무속성이 없다면 hit 시 사라짐
				bvisible = false;
				Scene_Manager::Take_Damage(this);
			}
			if (!(type & UINT(Bullet_Type::explosion)))
			{
				shared_ptr<Animation> effect = Effect_Manager::Load(Effect_File);
				effect->Position(sprites[current_idx]->Position());
				effect->Start();
			}
		}
	}
}

void Bullet::UpdateWorld()
{
	sprites[current_idx]->UpdateWorld();

}

void Bullet::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->ViewProjection(V, P);
	}
}

void Bullet::Scale(D3DXVECTOR3 val)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->Scale(val);
	}
}

void Bullet::Position(D3DXVECTOR3 val)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->Position(val);
	}
}

void Bullet::Rotator(D3DXVECTOR3 val)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->Rotator(val);
	}
}



/// <summary>
///////////////////////////////////////////// Bullet_Manager
/// </summary>
queue<shared_ptr<Bullet>> Bullet_Manager::bullets;
vector<shared_ptr<Bullet>> Bullet_Manager::bullets_vec;
map<wstring, queue <shared_ptr<Bullet>>> Bullet_Manager::m;

void Bullet_Manager::Create(int idx)
{
	queue<shared_ptr<Bullet>> test;
	bullets_vec.resize(idx * Bullet_FileCnt);
	int i = 0;
	for (; i < idx; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::Normal));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test.push(temp);
		temp->Init_Info(
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
		temp->Scale({ 30,30,1.f });
	}
	m[Digimon_Folder + L"길몬_브레스.png"] = test;

	queue<shared_ptr<Bullet>> test2;
	for (; i < idx * 2; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::explosion));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test2.push(temp);
		temp->Init_Info(
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
		temp->Scale({ 60,30,1.f });
	}
	m[Digimon_Folder + L"길몬_스킬브레스.png"] = test2;

	queue<shared_ptr<Bullet>> test3;
	bullets_vec.resize(idx * Bullet_FileCnt);
	for (; i < idx * 3; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::Normal));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test3.push(temp);
		temp->Init_Info(
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
		temp->Scale({ 53,19,1.f });
	}
	m[Digimon_Folder + L"레나몬_브레스.png"] = test3;

	queue<shared_ptr<Bullet>> test4;
	for (; i < idx * 4; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::through));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test4.push(temp);
		temp->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"레나몬_스킬브레스.png",//File
					1,									//Sprite_Cnt 
					0,									// Width
					0									// Height
			},
			35.f,  //Speed	
			{
			D3DXVECTOR4{0,0,7,6},
			},
			Effect_Folder + L"Hit_0.png"
			);

		temp->Scale({ 10,10,1.f });
	
	}
	m[Digimon_Folder + L"레나몬_스킬브레스.png"] = test4;


	queue<shared_ptr<Bullet>> test5;
	bullets_vec.resize(idx* Bullet_FileCnt);
	for (; i < idx * 5; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::Normal));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test5.push(temp);
		temp->Init_Info(
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
		temp->Scale({ 20,20,1.f });
	}
	m[Digimon_Folder + L"테리어몬_브레스.png"] = test5;

	queue<shared_ptr<Bullet>> test6;
	bullets_vec.resize(idx* Bullet_FileCnt);
	for (; i < idx * 6; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::through));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test6.push(temp);
		temp->Init_Info(
			Sprite_Info
			{
					Digimon_Folder + L"테리어몬_스킬브레스.png",//File
					5,									//Sprite_Cnt 
					0,									// Width
					0									// Height
			},
			35.f,		//Speed	
			{
			D3DXVECTOR4{0,0,19,22},
			D3DXVECTOR4{24,0,46,22},
			D3DXVECTOR4{52,0,72,22},
			D3DXVECTOR4{79,0,98,22},
			D3DXVECTOR4{105,0,125,22},
			},
			Effect_Folder + L"Hit_0.png"
			);
		temp->Scale({ 80,80,1.f });
	}
	m[Digimon_Folder + L"테리어몬_스킬브레스.png"] = test6;


	queue<shared_ptr<Bullet>> test7;
	bullets_vec.resize(idx* Bullet_FileCnt);
	for (; i < idx * 7; i++)
	{
		shared_ptr temp = make_shared<Bullet>(UINT(Bullet_Type::Normal));
		//bullets.push(temp);
		bullets_vec[i] = (temp);
		test7.push(temp);
		temp->Init_Info(
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
		temp->Scale({ 8,8,1.f });
	}
	m[Digimon_Folder + L"가르고몬_브레스.png"] = test7;

	

}

shared_ptr<Bullet> Bullet_Manager::Load(wstring imgfile)
{
	shared_ptr<Bullet> bullet = m[imgfile].front();
	m[imgfile].pop();
	m[imgfile].push(bullet);
	return bullet;
}

void Bullet_Manager::Render()
{
	for (int i = 0; i < BulletPool * Bullet_FileCnt; i++)
		bullets_vec[i]->Render();
}

void Bullet_Manager::Update()
{
	for (int i = 0; i < BulletPool * Bullet_FileCnt; i++)
		bullets_vec[i]->Update();
}

void Bullet_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < BulletPool * Bullet_FileCnt; i++)
		bullets_vec[i]->ViewProjection(V,P);
}

/*
*  Effect_Class
*/
map<wstring, queue <shared_ptr<Animation>>> Effect_Manager::m;
shared_ptr<Animation> Effect_Manager::level_up;
void Effect_Manager::Create(int idx)
{
	queue<shared_ptr<Animation>> test;
	int i = 0;
	for (; i < idx; i++)
	{
		shared_ptr<Animation> temp = make_shared<Animation>(
			Sprite_Info
			{
				Effect_Folder + L"Hit_0.png",
				5,
				192.f,
				192.f,
			},
			PlayMode::End
			);
		test.push(temp);
		temp->Scale({100,100,1});
	}
	m[Effect_Folder + L"Hit_0.png"] = test;
	queue<shared_ptr<Animation>> test2;
	for (; i < idx * 2; i++)
	{
		shared_ptr<Animation> temp = make_shared<Animation>(
			Sprite_Info
			{
				Effect_Folder + L"Explosion_0.png",
				5,
				25.f,
				25.f,
			},
			PlayMode::End
			);
		test2.push(temp);
		temp->Scale({ 150,150,1 });
	}
	m[Effect_Folder + L"Explosion_0.png"] = test2;


	shared_ptr<Animation> temp = make_shared<Animation>(
		Sprite_Info
		{
			Effect_Folder + L"Level_Up.png",
			4,
			16.f,
			16.f,
		},
		PlayMode::End
		);
	temp->Scale({ 150,150,1 });
	
	level_up = temp;
}

shared_ptr<Animation> Effect_Manager::Load(wstring imgfile)
{
	queue<shared_ptr<class Animation>> temp = m[imgfile];
	shared_ptr<Animation> result = temp.front();
	temp.pop();
	temp.push(result);
	return result;
}

void Effect_Manager::Render()
{
	for (auto iter = m.begin(); iter != m.end(); iter++) 
	{
		queue<shared_ptr<class Animation>> temp = iter->second;
		for(int i =0; i < temp.size(); i++)
		{
			auto front = temp.front();
			temp.pop();
			temp.push(front);
			if(front->Visible()) // 활성화 됬다면
				front->Render();
		}
	}
	if(level_up->Visible())
		level_up->Render();
	
}

void Effect_Manager::Update()
{
	for (auto iter = m.begin(); iter != m.end(); iter++)
	{
		queue<shared_ptr<class Animation>> temp = iter->second;
		for (int i = 0; i < temp.size(); i++)
		{
			auto front = temp.front();
			temp.pop();
			temp.push(front);
			if (front->Visible()) // 활성화 됬다면
				front->Update();
		}
	}
	if (level_up->Visible()) // 활성화 됬다면
		level_up->Update();
}

void Effect_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (auto iter = m.begin(); iter != m.end(); iter++)
	{
		queue<shared_ptr<class Animation>> temp = iter->second;
		for (int i = 0; i < temp.size(); i++)
		{
			auto front = temp.front();
			temp.pop();
			temp.push(front);
			front->ViewProjection(V,P);
		}
	}
	level_up->ViewProjection(V, P);
}

void Effect_Manager::Level_Up(D3DXVECTOR3 pos)
{
	level_up->Position(pos);
	level_up->Start();
}
