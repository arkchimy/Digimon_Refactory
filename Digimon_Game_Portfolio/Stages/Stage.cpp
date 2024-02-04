#include "stdafx.h"
#include "Stage.h"
#include "Map_Make/Map_Texture.h"
#include "Actors/Player.h"
#include "Digimon.h"
#include "Actors/Enemy.h"
#include "CastleGate.h"
Stage::Stage()
{

}


void Stage::Battle(bool val)
{

}

void Stage::Stage_Clear()
{
	Player->Stage_Clear();
	for (auto data : Enemy_vec)
		data->Battle(배틀종료);

}

void Stage::Wave_Clear()
{
}


void Stage::Render()
{
	for (int i = 0; i < layers.size(); i++)
	{
		layers[i]->Render();
	}
	for (int i = 0; i < ground.size(); i++)
	{
		ground[i]->Render();
	}
	
	Player->Render();
}

void Stage::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	// Map
	for (int i = 0; i < layers.size(); i++)
	{

		layers[i]->ViewProjection(V, P);
	}
	for (int i = 0; i < ground.size(); i++)
	{
		ground[i]->ViewProjection(V, P);

	}
	
	Player->ViewProjection(V, P);
}

void Stage::Take_Damage(Bullet* causer)
{
	Gate->Take_Damage(causer, {0,0,0});
}

void Stage::Add_Digimon(shared_ptr<Digimon> data)
{
	Player->Add_Digimon(data);
	
}


