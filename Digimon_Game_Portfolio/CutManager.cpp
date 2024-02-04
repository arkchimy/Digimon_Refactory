#include "stdafx.h"
#include "CutManager.h"
#include "Animation.h"
#include "Sprite.h"

float Cut_Speed = 2000.f;
extern bool bStop;
CutScean::CutScean()
{
	vector<shared_ptr<Sprite>> Fire_bg;
	Fire_bg.emplace_back(make_shared<Sprite>(Digimon_Folder + L"Skill_BG.png", 5, 530,520,780));

	Scene_bg = make_unique<Animation>(Fire_bg);
	Scene_bg->Position({ 0,0,0 });
	Scene_bg->Scale({ Width, 300, 1 });
}

CutScean::~CutScean()
{

}



void CutScean::Update()
{
	CheckNull(Scene);
	Scene->Update();
	D3DXVECTOR3 current_pos = Scene->Position();
	//화면 중간에서는 조금 느리게.
	if(current_pos.x  <= Scene->Scale().x * 0.3f && current_pos.x >= Scene->Scale().x * -0.3f)
		current_pos.x = current_pos.x + Time::Delta() * Cut_Speed * 0.1f;
	else
		current_pos.x = current_pos.x + Time::Delta() * Cut_Speed;
	Scene->Position(current_pos);
	if (Width / 2.f <= current_pos.x + Scene->Scale().x * -0.5f)
		Cut_End();


}

void CutScean::Render()
{
	CheckNull(Scene);
	Scene_bg->Render();
	Scene->Render();
}

void CutScean::Scale(D3DXVECTOR3 val)
{
	CheckNull(Scene);
	Scene->Scale(val);
}

void CutScean::Position(D3DXVECTOR3 val)
{
	CheckNull(Scene);
	Scene->Position(val);
}

void CutScean::Cut_Start(wstring cutfile)
{
	Scene =  CutManager::Load(cutfile);
	Scene->Position({ Width/-2.f - Scene->Scale().x * 0.5f, 0, 0 });
	Scene->Start();
	bStop = true;
}

void CutScean::Cut_End()
{
	Scene->Visible(false);
	Scene = nullptr;
	bStop = false;
}

void CutScean::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	Scene_bg->ViewProjection(V, P);
}


/*
* ManagerClass
*/
map<wstring, shared_ptr<Animation>> CutManager::m;

void CutManager::Create()
{
	vector<shared_ptr<Sprite>> Guilmon;
	Guilmon.emplace_back(make_shared<Sprite>(Digimon_Folder + L"Guilmon_Cut.png",137,102.f));
	m[L"Guilmon_Cut.png"]= make_shared<Animation>(Guilmon);
	m[L"Guilmon_Cut.png"]->Scale({ 300,300,1 });

	vector<shared_ptr<Sprite>> Renamon;
	Renamon.emplace_back(make_shared<Sprite>(Digimon_Folder + L"Renamon_Cut.png", 137, 102.f));
	m[L"Renamon_Cut.png"] = make_shared<Animation>(Renamon);
	m[L"Renamon_Cut.png"]->Scale({ 300,300,1 });


	vector<shared_ptr<Sprite>> Terriermon;
	Terriermon.emplace_back(make_shared<Sprite>(Digimon_Folder + L"Terriermon_Cut.png"));
	m[L"Terriermon_Cut.png"] = make_shared<Animation>(Terriermon);
	m[L"Terriermon_Cut.png"]->Scale({ 300,300,1 });

	
}

shared_ptr<class Animation> CutManager::Load(wstring file)
{
	return m[file];
}

void CutManager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (auto cut : m)
	{
		cut.second->ViewProjection(V, P);
	}
}

