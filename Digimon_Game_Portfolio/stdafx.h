#pragma once

#define _CRT_RAND_S

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <directxmath.h>
#include <windows.h>
#include <memory>
#include <wrl.h>

#include <assert.h>

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>

#include <functional>
#include <chrono>

#include <iostream>
using namespace std;

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

#include "Systems/Keyboard.h"
#include "Systems/Time.h"
#include "DirectWrite.h"

#include "Utilities/Math.h"
#include "Utilities/Path.h"
#include "Utilities/String.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Renders/Shader.h"


#define CheckTrue(p) { if(p == true) return;}
#define CheckFalse(p) { if(p == false) return;}
#define CheckNull(p) { if(p == nullptr) return;}

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }


// PoolSize  정의
#define DigimonPoolsize 10
#define Enemy_PoolSize 20
#define Poolsize 35 // queue_size of Objectpool
#define BulletPool 30 // 총알 Pool Size
#define CardPoolSize 2 // 카드 선택지에 최대 2개가 나옴.

#define Bullet_FileCnt 7

#define 기본SpritPos 700
#define 스킬SpritPos 800

#define LayerSize 2
#define GroundSize 1

#define LayerImage_Size 500
#define GroundImage_Size 500

#define 성장기 0x0001
#define 보스 0x0010

#define 배틀종료 false
#define 배틀시작 true

#define IDLE		 0
#define Walk		 1
#define Action		 2
#define Skill		 3
#define Hit			 4
#define Victory      5
#define Death		 6
#define PowerUP		 7

#define Sprite_Type  8 //   Idle /Walk / Action / Skill / Hit / Victory /Death

// Decal
extern float Decal_radius;
extern float Decal_alpha;
extern bool State_Clear;
extern float Sprite_Speed; // Sprite 애니메이션 속도


extern D3DXVECTOR3 Edit_Pos;
extern D3DXVECTOR3 Edit_Scale;
extern D3DXVECTOR3 Edit_Rot;
extern D3DXVECTOR3 Castle_Pos;

extern D3DXVECTOR2 Mouse_Pos;


///////////////////////////////////////////////////////////////////////////
// 경로 정리
const wstring Digimon_Folder = L"./_Textures/digimon/";

const wstring Textures = L"_Textures/";
const wstring Shaders = L"../_Shaders/";
const wstring Layer_Folder = L"./_Textures/layers/";

const wstring Map_Texture_Shader = L"../_Shaders/07_Texture.fx";
const wstring Player_Shader = L"../_Shaders/07_Texture.fx";
const wstring Texture_Shader = L"../_Shaders/07_Texture.fx";
const wstring Effect_Shader = L"../_Shaders/04_Effect.fx";
const wstring Card_Shader = L"../_Shaders/02_Card.fx";

const wstring TileSampling_Shader = L"../_Shaders/10_TileSampling.fx";

const wstring Layer = L"LayerImage_";
const wstring Ground = L"Ground_";

///////////////////////////////////////////////////////////////////////////
const INT Width = 1024;
const INT Height = 768;

extern HWND Hwnd;
extern wstring Title;

extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

//vertax
extern ID3D11VertexShader* VertexShader;
extern ID3D11PixelShader* PixelShader;
extern ID3D10Blob* VsBlob;
extern ID3D10Blob* PsBlob;


extern vector<glm::vec4> pixels;

extern Keyboard* Key;
extern bool R_Push;

//extern shared_ptr<Raytracer> raytracer;

extern float g_Gravity;

struct Vertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 Uv;
};



extern D3DXCOLOR TileSample_Lv;