#include "stdafx.h"
#include "Bottom_UI.h"
#include "Sprite.h"

bool Bottom_UI::bvictory = false;

Bottom_UI::Bottom_UI()
{
	m_buffer.resize(2);
	m_rotator.resize(2, { 0,0,0 });

	m_shader.push_back(new Shader(Shaders + L"03_Panel.fx"));
	m_shader.push_back(new Shader(Shaders + L"03_Panel.fx"));

	m_srv.emplace_back(Sprite_Manager::Load(Layer_Folder + L"Bottom_UI.png"));
	make_shared<Sprite>(m_buffer[0], Layer_Folder + L"Bottom_UI.png");

	m_srv.emplace_back(Sprite_Manager::Load(Layer_Folder + L"Game_Clear.png"));
	make_shared<Sprite>(m_buffer[1], Layer_Folder + L"Game_Clear.png");

	m_position.emplace_back( 0,-310,0 );
	m_scale.emplace_back( 1024, 150, 0 );

	m_position.emplace_back( 0,50,0 );
	m_scale.emplace_back( 300,100,0 );



	UpdateWorld();
}

Bottom_UI::~Bottom_UI()
{
	//SAFE_DELETE(sprite);
}

void Bottom_UI::Update()
{
	
}

void Bottom_UI::Render()
{
	for (int i = 0; i < 2; i++)
	{

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		m_shader[i]->AsShaderResource("Map")->SetResource(m_srv[i].Get());

		DeviceContext->IASetVertexBuffers(0, 1, m_buffer[i][0].GetAddressOf(), &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (i == 1 && !bvictory)
			break;
		m_shader[i]->Draw(0, 0, 6);
	}
}

void Bottom_UI::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < 2; i++)
	{
		m_shader[i]->AsMatrix("View")->SetMatrix(V);
		m_shader[i]->AsMatrix("Projection")->SetMatrix(P);
	}

}


void Bottom_UI::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;
	for (int i = 0; i < 2; i++)
	{
		D3DXMatrixScaling(&S, m_scale[i].x, m_scale[i].y, m_scale[i].z);
		D3DXMatrixRotationYawPitchRoll(&R, m_rotator[i].y, m_rotator[i].x, m_rotator[i].z);
		D3DXMatrixTranslation(&T, m_position[i].x, m_position[i].y, m_position[i].z);
		W = S * R * T;


		m_shader[i]->AsMatrix("World")->SetMatrix(W);
	}
}
