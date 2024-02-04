#include "stdafx.h"
#include "Title_Scene.h"
#include "Map_Make/Map_Texture.h"
#include "Actors/Player.h"


Title_Scene::Title_Scene()
{

}

Title_Scene::~Title_Scene()
{

}

void Title_Scene::Update()
{
	for (int i = 0; i < layers.size(); i++)
	{
		layers[i]->Animation(MainMap_Speed[i]);
	}
	for (int i = 0; i < ground.size(); i++)
	{
		ground[i]->Animation(MainMap_GSpeed[i]);
	}

	//Player
	{
		Player->Update();
	}
}

void Title_Scene::Init_Stage(shared_ptr<class Player> player)
{
	Player = player;
	
	//BackGround Layer
	{
		layers.resize(LayerSize);
		for (int i = 0; i < LayerSize; i++)
		{
			layers[i] = make_shared<Map_Texture>(Layer_Folder + L"Stage_00"  + L"/" + Layer + to_wstring(i) + L".png");
		}

	}
	// Ground
	{
		ground.resize(GroundSize);
		for (int i = 0; i < GroundSize; i++)
		{
			ground[i] = make_shared<Map_Texture>(Layer_Folder + L"Stage_00" + L"/" + Ground + to_wstring(i) + L".png");
		}

	}
	Player->Battle(배틀종료);
	
}

