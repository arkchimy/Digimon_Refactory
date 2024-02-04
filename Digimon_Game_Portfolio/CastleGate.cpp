#include "stdafx.h"
#include "CastleGate.h"
#include "Digimon.h"
#include "Map_Make/Map_Texture.h"
#include "Bullet.h"


D3DXVECTOR3 Castle_Pos = D3DXVECTOR3(-440, 65, 0);
float Castle_interver = 100.f;

CastleGate::CastleGate()
{
	// Ground
	{
		Castle = make_unique<Map_Texture>(Layer_Folder + L"Dungeon_01" + L"/" + Ground + to_wstring(0) + L".png");
		Castle->Scale(D3DXVECTOR3(850, 800, 1));
		Castle->Position(D3DXVECTOR3(-340, 65, 0));
		
	}
	position.resize(5);
	position[0] = D3DXVECTOR3(Castle_Pos.x, Castle_Pos.y, Castle_Pos.z);
	position[1] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver, Castle_Pos.z);
	position[2] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver, Castle_Pos.z);
	position[3] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver * 2, Castle_Pos.z);
	position[4] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver * 2, Castle_Pos.z);
}

CastleGate::~CastleGate()
{
}

void CastleGate::Render()
{
	Castle->Render();
}

void CastleGate::Update()
{
	Castle->Update();

}

void CastleGate::Animation()
{
	
}

void CastleGate::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	Castle->ViewProjection(V,P);
}

void CastleGate::Take_Damage(Bullet* causer, D3DXVECTOR3 dir)
{
	hp -= causer->Power();
}

bool CastleGate::IsDeathMode()
{
	return false;
}

vector<D3DXVECTOR3> CastleGate::Slot_Position()
{
	return position;
}
