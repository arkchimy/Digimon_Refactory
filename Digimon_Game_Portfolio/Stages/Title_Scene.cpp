#include "stdafx.h"
#include "Title_Scene.h"
#include "Map_Make/Map_Texture.h"
#include "Actors/Player.h"


Title_Scene::Title_Scene()
{

}

Title_Scene::~Title_Scene()
{
	cout << "title Scene �Ҹ��� ȣ��" << endl;
}

void Title_Scene::Update()
{
	

	//Player
	{
		Player->Update();
	}
}

void Title_Scene::Init_Stage(shared_ptr<class Player> player)
{
	Player = player;
	
	Player->Battle(��Ʋ����);
	
}

