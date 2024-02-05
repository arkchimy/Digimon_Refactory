#include "stdafx.h"
#include "Stage_01.h"

#include "Map_Make/Map_Texture.h"
#include "Actors/Player.h"
#include "CastleGate.h"

#include "Actors/Enemy.h"
#include "Actors/ABoss.h"

#include "Digimon.h"
#include "Reward_Card.h"
#include "UI/Bottom_UI.h"

int wave_info[Wave_SIZE] =
{
	3,
	5,
	9,
	12,
	15,
	17,
	20,
};

vector<wstring> files = {
	Layer_Folder + L"Dungeon_01" + L"/" + Layer + to_wstring(0) + L".png"
};
Stage_01::Stage_01()
	:Stage(files)
{
	Gate = new CastleGate();
}

Stage_01::~Stage_01()
{
	SAFE_DELETE(Gate);
}

void Stage_01::Init_Stage(shared_ptr<class Player> player)
{
	for (int i = 0; i < Enemy_PoolSize; i++)
	{
		int id = rand() % 2;
		if (id == 0)
			Enemy_pool.push(make_shared<Enemy>(Digimon_Folder + L"길몬.png", 40, 40, Guilmon, 성장기));
		if (id == 1)
			Enemy_pool.push(make_shared<Enemy>(Digimon_Folder + L"레나몬.png", Renamon_UV, Renamon, 성장기));
	}
	Player = player;
	
	init_Wave();
	Player->Slot_Set(Gate->Slot_Position());
	Player->Ready();

	boss_que.push(make_shared<ABoss>(Digimon_Folder + L"길몬.png", 40, 40, Guilmon, 보스));

	reward.resize(REWARD_SIZE);
	for(int i =0; i < REWARD_SIZE;i++)
		reward[i] = nullptr;
	Card_Manager::Create(Player->Player_Cards());

}
void Stage_01::Battle_Stage()
{
	Player->Battle(배틀시작);
	for (auto data : Enemy_vec)
		data->Battle(배틀시작);
}
void Stage_01::init_Wave()
{
	int cnt = wave_info[wave];
	for (int i = 0; i < cnt; i++)
	{
		shared_ptr<Enemy> front = Enemy_pool.front();
		Enemy_pool.pop();
		Enemy_pool.push(front);

		Enemy_vec.emplace_back(front);
	}
	float result = 0;
	for (auto data : Enemy_vec)
	{
		int i = rand() % 10;
		int dir = pow(-1, i);
		float interver = rand() % 100;
		result += interver;
		data->Position({ Width / 2.f + result + data->Scale().x * 0.5f ,float(rand() % 300) * dir + data->Scale().y * 0.5f , 0.f });
		data->Rotator(D3DXVECTOR3{ 0.f,180.f,0.f });
		
	}
	for (auto data : Enemy_vec)
		data->Battle(배틀종료);
}
void Stage_01::Boss_Appear()
{
	if (!boss_que.empty())
	{
		boss = boss_que.front();
		boss_que.pop();
		boss->Respawn();
	}
	
}
void Stage_01::Update()
{
	
	Gate->Update();
	Player->Update();

	if (reward[0] != nullptr)
	{
		for (auto data : reward) // Card
			data->Update();
		for (auto data : Enemy_vec) // Enemy Actor
			data->Update();
		return;
	}
	for (auto data : Enemy_vec)
	{
		data->Update();
	}
	CheckFalse(Enemy_vec[0]->bBattle);
	bclear = true;
	for (auto data : Enemy_vec)
	{
		if (bclear == true)
			bclear = data->IsDeathMode();
	}

	if (bclear) //  전부 다 죽었을 경우
	{
		if (wave >= Wave_SIZE)// 끝에 도달했다면
		{
			Stage_Clear();
			Bottom_UI::GameClear(true);
		}
		else 
		{
			if (boss != nullptr)
			{
				if (boss->IsDeathMode())
				{
					auto iter = find(Enemy_vec.begin(), Enemy_vec.end(), boss);
					Enemy_vec.erase(iter);
					boss = nullptr;
				}
			}
			Wave_Reward();
		}
	}

}

void Stage_01::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < shader_vec.size(); i++)
	{
		shader_vec[i]->AsShaderResource("Map")->SetResource(srv_vec[i]);

		DeviceContext->IASetVertexBuffers(0, 1, &buffer_vec[i], &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader_vec[i]->Draw(0, 0, 6);
	}

	Gate->Render();
	Player->Render();
	
	for (auto data : Enemy_vec)
		data->Render();


	CheckNull(reward[0]);
	for (auto data : reward)
		data->Render();
}



void Stage_01::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	
	for (int i = 0; i < shader_vec.size(); i++)
	{
		shader_vec[i]->AsMatrix("View")->SetMatrix(V);
		shader_vec[i]->AsMatrix("Projection")->SetMatrix(P);
	}

	Gate->ViewProjection(V, P);
	Player->ViewProjection(V, P);

	for (int i = 0; i < Enemy_PoolSize; i++)
	{
		auto front = Enemy_pool.front();
		Enemy_pool.pop();
		Enemy_pool.push(front);
		front->ViewProjection(V,P);
	}

	Card_Manager::ViewProjection(V, P);
	CheckTrue(boss_que.empty());

	for (int i = 0; i < BossQue_SIZE; i++)
	{
		auto front  = boss_que.front();
		boss_que.pop();
		boss_que.push(front);
		front->ViewProjection(V, P);
	}
}

D3DXVECTOR3 reward_pos[2] =
{
	{300,0,0},
	{-50,0,0},

};
void Stage_01::Wave_Reward()
{
	for (int i = 0; i < reward.size(); i++) 
	{
		reward[i] = Card_Manager::Load();
		reward[i]->Set_Owner(this);
		reward[i]->Visible(true);
		reward[i]->Position(reward_pos[i]);
	}
	Player->Battle(배틀종료);
	for (auto data : Enemy_vec)
		data->Battle(배틀종료);
	
}

void Stage_01::Wave_Clear()
{
	
	wave++;
	for (int i = 0; i < reward.size(); i++)
	{
		reward[i]->Visible(false);
		reward[i]->Position({2000,2000,0});
		reward[i] = nullptr;
	}
	
	int cnt = wave_info[wave] - wave_info[wave - 1];

	for (int i = 0; i < cnt; i++)
	{
		shared_ptr<Enemy> front = Enemy_pool.front();
		Enemy_pool.pop();
		Enemy_pool.push(front);

		Enemy_vec.emplace_back(front);
	}
	if (wave == Wave_SIZE) //마지막 스테이지
	{
		Boss_Appear();
		if (boss != nullptr) 
		{
			Enemy_vec.push_back(boss);
			boss->Position({ 0, 0, 0 });
		}
	}
	float result = 0;
	for (auto data : Enemy_vec)
	{
		int i = rand() % 10;
		int dir = pow(-1, i);
		float interver = rand() % 100;
		result += interver;
		data->Position({ Width / 2.f + result + data->Scale().x * 0.5f ,float(rand() % 350) * dir + data->Scale().y *0.5f , 0.f });
		data->Rotator(D3DXVECTOR3{ 0.f,180.f,0.f });
		data->Respawn();
		D3DXVECTOR3 pos;
		if (boss != nullptr)
		{
			if (data == boss)
			{
				 pos = data->Position();
			}
		}
	}
	

}
void Stage_01::Add_Digimon(shared_ptr<Digimon> data)
{
	int idx = Player->DigimonCnt();
	Player->Add_Digimon(data);

	data->MoveTo(Gate->Slot_Position()[idx]);
	data->Rotator({ 0, 180, 0 });
	Player->Battle(배틀시작);
	Wave_Clear();

}