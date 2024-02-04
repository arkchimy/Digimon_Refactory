#include "stdafx.h"
#include "Decal.h"
#include "Sprite.h"

Decal::Decal()
	:bVisible(false)
{
	sprite = make_unique<Sprite>(Decal_Folder + L"Circle.png", Decal_Shader);
}
Decal::~Decal()
{
}
void Decal::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	sprite->ViewProjection(V, P);
}

void Decal::Update()
{
	CheckFalse(bVisible);
	D3DXVECTOR3 temp;
	temp.x = Width * 0.5f;
	temp.y = Height * 0.5f;
	sprite->Position({ Mouse_Pos.x - temp.x, -(Mouse_Pos.y - temp.y), 0 });
}

void Decal::Render()
{
	CheckFalse(bVisible);
	sprite->Render();
}

void Decal::Scale(D3DXVECTOR3 val)
{
	sprite->Scale(val);
}

void Decal::Position(D3DXVECTOR3 val)
{
	sprite->Position(val);
}
