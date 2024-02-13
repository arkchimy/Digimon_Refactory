#include "stdafx.h"
#include "CastleGate.h"
#include "Sprite.h"

D3DXVECTOR3 Castle_Pos = D3DXVECTOR3(-440, 65, 0);
float Castle_interver = 100.f;

CastleGate::CastleGate()
{

	shader = make_unique<Shader>(Texture_Shader);
	
	srv = Sprite_Manager::Load(Layer_Folder + L"Dungeon_01" + L"/" + Ground + to_wstring(0) + L".png");
	make_unique<Sprite>(buffer, Layer_Folder + L"Dungeon_01" + L"/" + Ground + to_wstring(0) + L".png");
	// Ground
	{
		m_scale = D3DXVECTOR3(850, 800, 1);
		m_position = D3DXVECTOR3(-340, 65, 0);
		m_rotator = { 0,0,0 };
	}
	UpdateWorld();

	slot_position.resize(5);
	slot_position[0] = D3DXVECTOR3(Castle_Pos.x, Castle_Pos.y, Castle_Pos.z);
	slot_position[1] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver, Castle_Pos.z);
	slot_position[2] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver, Castle_Pos.z);
	slot_position[3] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y - Castle_interver * 2, Castle_Pos.z);
	slot_position[4] = D3DXVECTOR3(Castle_Pos.x - 10.f, Castle_Pos.y + Castle_interver * 2, Castle_Pos.z);

	slot_use.resize(5);
}

CastleGate::~CastleGate()
{
}

void CastleGate::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	
	shader->AsShaderResource("Map")->SetResource(srv.Get());

	DeviceContext->IASetVertexBuffers(0, 1, buffer[0].GetAddressOf(), &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
	
}


void CastleGate::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, m_rotator.y, m_rotator.x, m_rotator.z);
	D3DXMatrixTranslation(&T, m_position.x, m_position.y, m_position.z);
	

	W = S * R * T;

	shader->AsMatrix("World")->SetMatrix(W);
}

void CastleGate::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{

	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

}


D3DXVECTOR3 CastleGate::Slot_Position()
{
	for (int i = 0; i < slot_position.size(); i++)
	{
		//슬롯이 미사용중이면
		if (!slot_use[i])
		{
			slot_use[i] = true;
			return slot_position[i];
		}
	}
	return {0,0,0};
}

void CastleGate::ReturnSlot(D3DXVECTOR3 slotpos)
{
	for (int i = 0; i < slot_position.size(); i++)
	{
		if (slot_position[i] == slotpos) 
		{
			slot_use[i] = false;
			break;
		}
	}
}

