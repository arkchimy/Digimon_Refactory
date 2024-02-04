#include "stdafx.h"
#include "Bottom_UI.h"
#include "Sprite.h"

bool Bottom_UI::bvictory;
Bottom_UI::Bottom_UI(wstring file)
{
	sprite = new Sprite(file);
	sprite->Position({ 0,-310,0 });
	sprite->Scale({ 1024,150,0 });

	victory = new Sprite(Layer_Folder + L"Game_Clear.png");
	victory->Position({ 0,50,0 });
	victory->Scale({300,100,0});
}

Bottom_UI::~Bottom_UI()
{
	SAFE_DELETE(sprite);
}

void Bottom_UI::Update()
{
	
}

void Bottom_UI::Render()
{
	sprite->Render();
	CheckFalse(bvictory);
	victory->Render();
}

void Bottom_UI::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	sprite->ViewProjection(V, P);
	victory->ViewProjection(V, P);
}

void Bottom_UI::Scael(D3DXVECTOR3 val)
{
	sprite->Scale(val);
}

void Bottom_UI::Position(D3DXVECTOR3 val)
{
	sprite->Position(val);
}
