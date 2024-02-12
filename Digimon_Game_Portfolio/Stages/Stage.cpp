#include "stdafx.h"
#include "Stage.h"
#include "Map_Make/Map_Texture.h"
#include "Actors/Player.h"
#include "Digimon.h"
#include "Actors/Enemy.h"
#include "CastleGate.h"

#include "Sprite.h"




wstring Title_bg0 = Layer_Folder + L"Stage_00" + L"/" + Layer + to_wstring(0) + L".png";
wstring Title_bg1 = Layer_Folder + L"Stage_00" + L"/" + Layer + to_wstring(1) + L".png";
wstring Title_ground0 = Layer_Folder + L"Stage_00" + L"/" + Ground + to_wstring(0) + L".png";

Stage::Stage()
{
	vector<wstring> imagefiles;
	imagefiles.reserve(3);
	imagefiles.push_back(Title_bg0);
	imagefiles.push_back(Title_bg1);
	imagefiles.push_back(Title_ground0);

	vector<wstring> shaderfiles =
	{
		Texture_Shader,
		Texture_Shader,
		Texture_Shader,
	};

	CreateShaderAndBuffer(imagefiles, shaderfiles);
}

Stage::Stage(vector<wstring> imagefiles, vector<wstring> shaderfiles)
{
	CreateShaderAndBuffer(imagefiles, shaderfiles);
}

Stage::~Stage()
{
	SAFE_DELETE(Gate);
}


void Stage::CreateShaderAndBuffer(vector<wstring> imagefiles, vector<wstring> shaderfiles)
{

	shader_vec.reserve(imagefiles.size());
	position.reserve(imagefiles.size());
	scale.reserve(imagefiles.size());
	rotator.reserve(imagefiles.size());
	srv_vec.resize(imagefiles.size());
	
	for (int i=0; i < imagefiles.size(); i++)
	{
		srv_vec[i] = Sprite_Manager::Load(imagefiles[i]);
		make_unique<Sprite>( buffer_vec, imagefiles[i], shaderfiles[i]);

		shader_vec.push_back(make_unique<Shader>(shaderfiles[i]));
		position.push_back({ 0,0,0 });
		scale.push_back({ Width,Height,1 });
		rotator.push_back({ 0,0,0 });
		
	}
	UpdateWorld();
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


void Stage::Render()
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

	Player->Render();
}

void Stage::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < shader_vec.size(); i++)
	{
		shader_vec[i]->AsMatrix("View")->SetMatrix(V);
		shader_vec[i]->AsMatrix("Projection")->SetMatrix(P);
	}
	Player->ViewProjection(V, P);
}

void Stage::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;
	for (int i = 0; i < shader_vec.size(); i++)
	{
		D3DXMatrixScaling(&S, scale[i].x, scale[i].y, scale[i].z);
		D3DXMatrixRotationYawPitchRoll(&R, rotator[i].y, rotator[i].x, rotator[i].z);
		D3DXMatrixTranslation(&T, position[i].x, position[i].y, position[i].z);
		W = S * R * T;

		shader_vec[i]->AsMatrix("World")->SetMatrix(W);
	}
}





void Stage::Take_Damage(Bullet* causer)
{
	//Gate->Take_Damage(causer, {0,0,0});
}

void Stage::Add_Digimon(shared_ptr<Digimon> data)
{
	Player->Add_Digimon(data);
}


