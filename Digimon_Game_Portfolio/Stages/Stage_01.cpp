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

#include <random>

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

D3DXCOLOR TileSample_Lv = { 1,0,0,0 };

vector<wstring> imagegefiles = {
	Layer_Folder + L"Dungeon_01" + L"/" + Layer + to_wstring(0) + L".png"
};
vector<wstring> shaderfiles = {
	TileSampling_Shader
};
Stage_01::Stage_01()
	:Stage(imagegefiles, shaderfiles)
{
	Gate = new CastleGate();
	reward.reserve(CardPoolSize);
	for (int i = 0; i < CardPoolSize; i++)
	{
		reward.push_back(make_unique<Reward_Card>());

	}

	CreateSamplerState();
}

Stage_01::~Stage_01()
{
	SAFE_DELETE(Gate);
}

void Stage_01::Init_Stage(shared_ptr<class Player> player)
{

	Player = player;
	init_Wave();

	Player->Slot_Set(Gate->Slot_Position());
	Player->Ready();

	boss_que.push(make_shared<ABoss>(Digimon_Folder + L"길몬.png", 40, 40, Guilmon, 보스));

	Card_Manager::Create(Player->Player_Cards());
	m_State = StageState::Rest;

}
void Stage_01::Battle_Stage()
{
	m_State = StageState::Battle;
	Player->Battle(배틀시작);
	for (auto data : Enemy_vec)
		data->Battle(배틀시작);

}
void Stage_01::init_Wave()
{
	int cnt = wave_info[wave];
	for (int i = 0; i < cnt; i++)
	{
		unsigned int    number;

		rand_s(&number);
		number = (unsigned int)((double)number /
			((double)UINT_MAX + 1) * 2.f);

		shared_ptr<Enemy> temp;
		if (number == 0)
			temp = Enemy_Manager::Load(Digimon_Folder + L"길몬.png");
		else if (number == 1)
			temp = Enemy_Manager::Load(Digimon_Folder + L"레나몬.png");

		Enemy_vec.push_back(temp);
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
		boss->Scale({150,150,1});
	}

}
void Stage_01::ClickEvent()
{
	for (int i = 0; i < CardPoolSize; i++)
	{
		if (reward[i]->MouseOver())
		{
			Add_Digimon(reward[i]->CardDigimon());
			reward[i]->Use_Card();

			for (int k = 0; k < CardPoolSize; k++)
				reward[k]->Visible(false);
			break;
		}
	}
}
void Stage_01::Update()
{
	

	Player->Update();

	for (int i = 0; i < reward.size(); i++)
	{
		reward[i]->Update();
	}

	for (auto data : Enemy_vec)
	{
		data->Update();
	}
	CheckTrue(m_State == StageState::Rest);

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


float Decal_radius = 0.244f;
float Decal_alpha = 0.512f;


void Stage_01::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	float aspect = (float)Width / Height;
	D3DXVECTOR4 m_mouse_pos;
	m_mouse_pos.x = (float)Mouse_Pos.x / Width * aspect;
	m_mouse_pos.y = (float)Mouse_Pos.y / Height;
	m_mouse_pos.z = Decal_radius;
	m_mouse_pos.w = Decal_alpha;

	for (int i = 0; i < shader_vec.size(); i++)
	{
		shader_vec[i]->AsShaderResource("Map")->SetResource(srv_vec[i]);
		shader_vec[i]->AsSampler("Sampler")->SetSampler(0, m_samplerState);
		shader_vec[i]->AsVector("xSplit")->SetFloatVector(TileSample_Lv);
		shader_vec[i]->AsVector("MousePos")->SetFloatVector(m_mouse_pos);

		DeviceContext->IASetVertexBuffers(0, 1, &buffer_vec[i], &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader_vec[i]->Draw(0, 0, 6);
	}

	Gate->Render();
	Player->Render();

	for (auto data : Enemy_vec)
	{
		data->Render();
	}

	for (int i = 0; i < reward.size(); i++)
	{
		reward[i]->Render();
	}

	//for (auto data : reward)
	//	data->Render();
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


	Enemy_Manager::ViewProjection(V, P);


	Card_Manager::ViewProjection(V, P);
	CheckTrue(boss_que.empty());

	for (int i = 0; i < BossQue_SIZE; i++)
	{
		auto front = boss_que.front();
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
	m_State = StageState::Rest;
	for (int i = 0; i < reward.size(); i++)
	{
		reward[i]->Load();
		reward[i]->Position(reward_pos[i]);
	}
	Player->Battle(배틀종료);


}

void Stage_01::Wave_Clear()
{
	m_State = StageState::Battle;
	wave++;
	for (int i = 0; i < reward.size(); i++)
	{
		reward[i]->Visible(false);
		reward[i]->Position({ 2000,2000,0 });

	}

	int cnt = wave_info[wave] - wave_info[wave - 1];

	for (int i = 0; i < cnt; i++)
	{
		unsigned int    number;

		rand_s(&number);
		number = (unsigned int)((double)number /
			((double)UINT_MAX + 1) * 4.f);

		shared_ptr<Enemy> temp;
		if (number == 0)
			temp = Enemy_Manager::Load(Digimon_Folder + L"길몬.png");
		else if (number == 1)
			temp = Enemy_Manager::Load(Digimon_Folder + L"레나몬.png");
		else if (number == 2)
			temp = Enemy_Manager::Load(Digimon_Folder + L"테리어몬.png");
		else if (number == 3)
			temp = Enemy_Manager::Load(Digimon_Folder + L"가르고몬.png");

		Enemy_vec.push_back(temp);
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
		data->Position({ Width / 2.f + result + data->Scale().x * 0.5f ,float(rand() % 350) * dir + data->Scale().y * 0.5f , 0.f });
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
	Player->Add_Digimon(data);

	data->MoveTo(Gate->Slot_Position());
	data->Rotator({ 0, 180, 0 });

	Player->Battle(배틀시작);
	Wave_Clear();

}
void Stage_01::ReturnSlot(D3DXVECTOR3 slotpos)
{
	Gate->ReturnSlot(slotpos);
}
void Stage_01::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // 3차원인듯
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Device->CreateSamplerState(&samplerDesc, &m_samplerState);

}
