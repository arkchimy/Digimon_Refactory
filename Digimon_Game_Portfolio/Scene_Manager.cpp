#include "stdafx.h"
#include "Scene_Manager.h"
#include "Actors/Player.h"


#include "Stages/Stage.h"
#include "Stages/Title_Scene.h"
#include "Stages/Stage_01.h"

#define STAGE_CNT 1

class Stage* Scene_Manager::Current_Scene = nullptr;
shared_ptr<class Player> Scene_Manager::Player = nullptr;

Scene_Manager::Scene_Manager()
{
	Current_Scene = new Title_Scene();

	Init_Stage();
}

void Scene_Manager::Init_Stage()
{
	Current_Scene->Init_Stage(Player);
}

void Scene_Manager::Start_Stage()
{
	Current_Scene->Battle_Stage();
}

void Scene_Manager::Update()
{
	Current_Scene->Update();
}

void Scene_Manager::Render()
{
	Current_Scene->Render();
}

void Scene_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	Current_Scene->ViewProjection(V,P);

}

void Scene_Manager::Take_Damage(Bullet* causer)
{
	Current_Scene->Take_Damage(causer);
}

vector<shared_ptr<class Enemy>> Scene_Manager::Get_Enemies()
{
	return Current_Scene->Enemy_vec;
}

void Scene_Manager::StandBy_Stage(int stage_num)
{
	if (stage_num) 
	{
		SAFE_DELETE(Current_Scene);
		Current_Scene = new Stage_01();
	}
	Init_Stage();
}

void Scene_Manager::Stage_Complete(bool val)
{
	
}

void Scene_Manager::ClickEvent()
{
	Current_Scene->ClickEvent();
}

void Scene_Manager::ReturnSlot(D3DXVECTOR3 slotpos)
{
	Current_Scene->ReturnSlot(slotpos);
}

