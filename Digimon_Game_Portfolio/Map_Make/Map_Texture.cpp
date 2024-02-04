#include "stdafx.h"
#include "Map_Texture.h"
#include "Sprite.h"

Map_Texture::Map_Texture(wstring imgFile, wstring shaderFile)
{
	Map = make_unique<Sprite>(imgFile, shaderFile);
	Swap_Map = make_unique<Sprite>(imgFile, shaderFile); // chain 스왑 용도

	Map_Pos = { 0,0,0 };

	Map->Scale({ Width,Height,1 });
	Swap_Map->Scale({ Width,Height,1 });
}

Map_Texture::~Map_Texture()
{
}

void Map_Texture::Update()
{
}

void Map_Texture::Render()
{
	Map->Render();
	Swap_Map->Render();
}

void Map_Texture::Position(D3DXVECTOR3 val)
{
	Map->Position(val);
	Swap_Map->Position(val);
	UpdateWorld();
}

void Map_Texture::Scale(D3DXVECTOR3 val)
{
	Map->Scale(val);
	Swap_Map->Scale(val);
	UpdateWorld();
}

void Map_Texture::Animation(D3DXVECTOR3 speed)
{
	Map_Pos = Map->Position();
	Map->Position(Map_Pos.x + speed.x * Time::Delta(), Map_Pos.y + speed.y * Time::Delta(), Map_Pos.z + speed.z * Time::Delta());

	Map_Pos = Map->Position();
	Move_BackPosition(Map_Pos);

	
	Swap_Map->Position(Swap_Map_Pos);

	ScreenRange_OutChk();

}

void Map_Texture::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	Map->ViewProjection(V,P);
	Swap_Map->ViewProjection(V, P);
}

void Map_Texture::UpdateWorld()
{
	Map->UpdateWorld();
	Swap_Map->UpdateWorld();
}

void Map_Texture::Move_BackPosition(D3DXVECTOR3 val)
{
	Swap_Map_Pos.x = val.x + Width;
	Swap_Map_Pos.y = val.y ;
	Swap_Map_Pos.z = val.z ;

}

void Map_Texture::ScreenRange_OutChk()
{
	if (Map_Pos.x + Map->Scale().x / 2.f <=  (-Width) / 2.f)
		Map->Position(0, Map_Pos.y, Map_Pos.z);
}
