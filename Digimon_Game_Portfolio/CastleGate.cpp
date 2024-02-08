#include "stdafx.h"
#include "CastleGate.h"
#include "Digimon.h"
#include "Map_Make/Map_Texture.h"
#include "Bullet.h"


#include "Sprite.h"
#include "Renders/Shader.h"

D3DXVECTOR3 Castle_Pos = D3DXVECTOR3(-440, 65, 0);
float Castle_interver = 100.f;

CastleGate::CastleGate()
{

	shader = make_unique<Shader>(Texture_Shader);
	
	srv = Sprite_Manager::Load(Layer_Folder + L"Dungeon_01" + L"/" + Ground + to_wstring(0) + L".png");
	make_unique<Sprite>(buffer, Layer_Folder + L"Dungeon_01" + L"/" + Ground + to_wstring(0) + L".png");
	// Ground
	{
		scale = D3DXVECTOR3(850, 800, 1);
		position = D3DXVECTOR3(-340, 65, 0);
		rotator = { 0,0,0 };
	}
	UpdateWorld();

	slot_position.resize(5);
	slot_position[0] = D3DXVECTOR3(Castle_Pos.x, Castle_Pos.y, Castle_Pos.z);
	slot_position[1] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver, Castle_Pos.z);
	slot_position[2] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver, Castle_Pos.z);
	slot_position[3] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver * 2, Castle_Pos.z);
	slot_position[4] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver * 2, Castle_Pos.z);
}

CastleGate::~CastleGate()
{
}

void CastleGate::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	
	shader->AsShaderResource("Map")->SetResource(srv);

	DeviceContext->IASetVertexBuffers(0, 1, &buffer[0], &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
	
}

void CastleGate::Update()
{
	//Castle->Update();

}

void CastleGate::Animation()
{
	
}

void CastleGate::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R,rotator.y,rotator.x,rotator.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	

	W = S * R * T;

	shader->AsMatrix("World")->SetMatrix(W);
}

void CastleGate::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{

	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

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
	return slot_position;
}
