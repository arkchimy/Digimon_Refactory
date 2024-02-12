#include "stdafx.h"
#include "Device.h"

#include "Actors/Player.h"
#include "Stages/Stage.h"
#include "Digimon.h"
#include "Actors/Enemy.h"

#include "Bullet.h"
#include "Scene_Manager.h"
#include "CastleGate.h"

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

class Bottom_UI* bottom_UI;


void InitScene()
{
	Digimon_Manager::CreatePooling(DigimonPoolsize);
	Enemy_Manager::CreatePooling(Enemy_PoolSize);

	player = make_shared<Player>();
	Scene_Manager::Player = player;
	stage = new Scene_Manager();

	ViewProjection_Init();


	stage->ViewProjection(V, P);


	Bullet_Manager::Create(BulletPool);
	Bullet_Manager::ViewProjection(V, P);

	Effect_Manager::Create(Poolsize);
	Effect_Manager::ViewProjection(V, P);

	
	bottom_UI = new Bottom_UI();
	bottom_UI->ViewProjection(V, P);
}

void DestroyScene()
{

}

bool chk = false;
void Update()
{
	
	if (!bStop)
	{
		stage->Update();
		stage->ViewProjection(V, P);

		Bullet_Manager::Update();
		Effect_Manager::Update();
	}
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
	ImGui::SliderFloat("Decal_radius", &Decal_radius, 0, 10);
	ImGui::SliderFloat("Decal_alpha", &Decal_alpha, 0, 1);


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
	D3DXMatrixOrthoOffCenterLH(&P, -(float)Width / 2, (float)Width / 2, -(float)Height / 2, (float)Height / 2, -1, 1);

}