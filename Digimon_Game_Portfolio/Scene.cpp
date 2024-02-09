#include "stdafx.h"
#include "Device.h"

#include "Actors/Player.h"
#include "Stages/Stage.h"
#include "Digimon.h"
#include "Bullet.h"
#include "Scene_Manager.h"
#include "CastleGate.h"
#include "Decal.h"
#include "CutManager.h"
#include "UI/Bottom_UI.h"

shared_ptr<Player> player;
D3DXMATRIX V, P;
D3DXVECTOR3 Edit_Rot = { 0,0,0 };

float Edit_Speed;
float Sprite_Speed = 9.5f;

bool State_Clear = false;
bool bDrawBound = false;
bool bStop = false; //Skill Stop


Scene_Manager* stage;

CutScean* Skill_Cut;
class Bottom_UI* bottom_UI;

void InitScene()
{
	Digimon_Manager::CreatePooling(DigimonPoolsize);

	player = make_shared<Player>();
	Scene_Manager::Player = player;
	stage = new Scene_Manager();

	ViewProjection_Init();




	stage->ViewProjection(V, P);



	Bullet_Manager::Create(BulletPool);
	Bullet_Manager::ViewProjection(V, P);

	Effect_Manager::Create(Poolsize);
	Effect_Manager::ViewProjection(V, P);

	CutManager::Create();
	CutManager::ViewProjection(V, P);

	
	Skill_Cut = new CutScean();
	Skill_Cut->ViewProjection(V, P); // CutScene BG
	
	bottom_UI = new Bottom_UI(Layer_Folder + L"Bottom_UI.png");
	bottom_UI->ViewProjection(V, P);
}

void DestroyScene()
{
	//SAFE_DELETE(modack);
	SAFE_DELETE(Skill_Cut);
}

bool chk = false;
void Update()
{
	
	if (!bStop)
	{
		stage->Update();
		stage->ViewProjection(V, P);
		//Digimon_Manager::ViewProjection(V, P);
		//modack->Update();
		Bullet_Manager::Update();
		Effect_Manager::Update();
	}
	Skill_Cut->Update();
	bottom_UI->Update();


}


void Render()
{
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 0, 0);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		ImGui_Event();
		stage->Render();
		
		//modack->Render();
	}
	Bullet_Manager::Render();
	Effect_Manager::Render();
	bottom_UI->Render();

	Skill_Cut->Render();
	ImGui::Render();
	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect; // text À§Ä¡
		rect.left = 0;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

	}

	DirectWrite::GetDC()->EndDraw();

	SwapChain->Present(0, 0);

}

extern D3DXVECTOR3 Castle_Pos ;
extern float Castle_interver;

void ImGui_Event()
{
	/*ImGui::SliderFloat2("Edit_Pos", Edit_Pos, -Width, Width);
	ImGui::SliderFloat2("Edit_Scale", Edit_Scale, -Width, Width);
	ImGui::SliderFloat3("Edit_Rot", Edit_Rot, -180, 180);
	ImGui::SliderFloat("Edit_Speed", &Edit_Speed, 1, 180);
	Sprite_Speed
	ImGui::SliderFloat("Castle_interver", &Castle_interver,0.f,500.f);
	*/
	

	ImGui::SliderFloat("TileSample_Lv", &TileSample_Lv.r, 1, 10);

	if (ImGui::Button("Standby_Stage"))
	{
		Scene_Manager::StandBy_Stage(1);
		stage->ViewProjection(V, P);
	}
	if (ImGui::Button("Battle_Start"))
	{
		Scene_Manager::Start_Stage();
	}
	if (ImGui::Button("State_Clear"))
	{
		//stage->Stage_Clear();
	}
	ImGui::Checkbox("DrawBoundBox", &bDrawBound);
	
	/*for (int i = 0; i < LayerSize; i++) 
	{
		string name = "map_speed" + to_string(i);
		ImGui::SliderFloat(name.c_str(), &Speed[i].x, -1, 0);
	}*/
	
}

void ViewProjection_Init()
{
	//View
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 at(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&V, &eye, &at, &up);

	//Projection
	//D3DXMatrixOrthoOffCenterLH(&P, 0, (float)Width, 0, (float)Height, -1, 1);
	D3DXMatrixOrthoOffCenterLH(&P, -(float)Width / 2, (float)Width / 2, -(float)Height / 2, (float)Height / 2, -1, 1);

}