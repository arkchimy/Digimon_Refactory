#include "stdafx.h"
#include "Device.h"
#include "Actors/Player.h"
#include "Reward_Card.h"

#include "Scene_Manager.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	InitWindow(hInstance, nShowCmd);
//	InitDirect3D();
//
//	Running();
//
//	Destroy();
//
//	return 0;
//}
int main()
{

	InitWindow();
	InitDirect3D();

	Running();

	Destroy();

	return 0;

}



HWND Hwnd = NULL;
wstring Title = L"Digimon Game";

IDXGISwapChain* SwapChain;
ID3D11Device* Device;
ID3D11DeviceContext* DeviceContext;
ID3D11RenderTargetView* RTV;

ID3D11VertexShader* VertexShader;
ID3D11PixelShader* PixelShader;
ID3D10Blob* VsBlob;
ID3D10Blob* PsBlob;

Keyboard* Key;
bool R_Push ;
bool NumKey_1 = false;
bool NumKey_2 = false;
bool NumKey_3 = false;

D3DXVECTOR2 Mouse_Pos;
extern shared_ptr<Player> player;


void InitWindow()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),
					CS_CLASSDC,
					WndProc,
					0L,
					0L,
					GetModuleHandle(NULL),
					NULL,
					NULL,
					NULL,
					NULL,
					L"Archimy_Framework", // lpszClassName, L-string
					NULL };

	// The RegisterClass function has been superseded by the RegisterClassEx
	// function.
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
	if (!RegisterClassEx(&wc))
	{
		cout << "RegisterClassEx() failed." << endl;
		return;
	}

	// 툴바까지 포함한 윈도우 전체 해상도가 아니라
	// 우리가 실제로 그리는 해상도가 width x height가 되도록
	// 윈도우를 만들 해상도를 다시 계산해서 CreateWindow()에서 사용

	// 우리가 원하는 그림이 그려질 부분의 해상도
	RECT wr = { 0, 0, Width, Height };

	// 필요한 윈도우 크기(해상도) 계산
	// wr의 값이 바뀜
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	// 윈도우를 만들때 위에서 계산한 wr 사용
	Hwnd = CreateWindow(wc.lpszClassName, L"Archimy_Framework",
		WS_OVERLAPPEDWINDOW,
		100, // 윈도우 좌측 상단의 x 좌표
		100, // 윈도우 좌측 상단의 y 좌표
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도
		NULL, NULL, wc.hInstance, NULL);

	if (!Hwnd)
	{
		cout << "CreateWindow() failed." << endl;
		return ;
	}

	ShowWindow(Hwnd, SW_SHOWDEFAULT);
	UpdateWindow(Hwnd);

	return ;
}

void InitWindow(HINSTANCE hInstance, int ShowWnd)
{
	
	//Register Class
	{
		WNDCLASSEX wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = NULL;
		wndClass.cbWndExtra = NULL;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = Title.c_str();
		wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		WORD check = RegisterClassEx(&wndClass);
		assert(check != 0);
	}

	//Create Handle
	{
		Hwnd = CreateWindowEx
		(
			NULL,
			Title.c_str(),
			Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			Width, Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);
		assert(Hwnd != NULL);
	}

	RECT rect = { 0,0, (LONG)Width, (LONG)Height };
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)Height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		Hwnd,
		centerX, centerY,
		rect.right - rect.left, rect.bottom - rect.top,
		TRUE
	);

	ShowWindow(Hwnd, ShowWnd);
	UpdateWindow(Hwnd);
}

void InitDirect3D()
{
	
	/*
		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &desc, &SwapChain, &Device, NULL, &DeviceContext
		);
		assert(SUCCEEDED(hr));
	}
	*/
	
	//Create Device and DeviceContext, SwapChain
	{
		DXGI_MODE_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferDesc = bufferDesc;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = Hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
			D3D11_SDK_VERSION, &desc, &SwapChain, &Device, NULL, &DeviceContext
		);
		assert(SUCCEEDED(hr));
	}

	CreateBackBuffer();
}

void Destroy()
{
	SAFE_RELEASE(VertexShader);
	SAFE_RELEASE(PixelShader);
	SAFE_RELEASE(VsBlob);
	SAFE_RELEASE(PsBlob);
	

	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(RTV);
}

WPARAM Running()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Time::Create();
	Time::Get()->Start();

	ImGui::Create(Hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark();
	DirectWrite::Create();

	//Key = new Keyboard;

	InitScene();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Time::Get()->Update();
			Update();
			ImGui::Update();
			Render();
		}
	}
	DestroyScene();


	Time::Delete();
	SAFE_DELETE (Key);

	DirectWrite::Delete();
	ImGui::Delete();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc((UINT*)hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
	{
		// 추후에 Width 와 height 변경하기
		if (Device != NULL)
		{
			ImGui::Invalidate();

			DeleteBackBuffer();
			DirectWrite::DeleteBackBuffer();

			HRESULT hr = SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
			assert(SUCCEEDED(hr));

			DirectWrite::CreateBackBuffer();
			CreateBackBuffer();

			ImGui::Validate();

		}

	}
	break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		if (wParam == 0x31)
		{
			NumKey_1 = true;
			NumKey_2 = false;
			NumKey_3 = false;
			player->Decal_Visible(1, true);
		}
		if (wParam == 0x32)
		{
			NumKey_1 = false;
			NumKey_2 = true;
			NumKey_3 = false;
			player->Decal_Visible(2, true);
		}
		if (wParam == 0x33)
		{
			NumKey_1 = false;
			NumKey_2 = false;
			NumKey_3 = true;
			player->Decal_Visible(3, true);
		}
		return 0;
	case WM_KEYUP:
		/*if (wParam == 0x31)
		{
			NumKey_1 = false;
		}*/
		return 0;
	case WM_MOUSEMOVE:
		if (msg == WM_MOUSEMOVE)
		{
			Mouse_Pos.x = (float)LOWORD(lParam);
			Mouse_Pos.y = (float)HIWORD(lParam);
		}
		return 0;
	case WM_LBUTTONDOWN:
		if (NumKey_1) 
		{
			player->Use_Skill(1, Mouse_Pos);
			player->Decal_Visible(1, false);
			NumKey_1 = false;
		}
		if (NumKey_2)
		{
			player->Use_Skill(2, Mouse_Pos);
			player->Decal_Visible(2, false);
			NumKey_2 = false;
		}
		if (NumKey_3)
		{
			player->Use_Skill(3, Mouse_Pos);
			player->Decal_Visible(3, false);
			NumKey_3 = false;
		}
		//Card_Manager::Use_Card();
		Scene_Manager::ClickEvent();
		player->ClickEvent();
		return 0;

	case WM_LBUTTONUP:
		player->ClickUp_Event();
		return 0;
	case WM_RBUTTONDOWN:
		R_Push = true;
		return 0;

	case WM_RBUTTONUP:
		R_Push = false;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}


	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void CreateBackBuffer()
{
	{
		HRESULT hr;

		ID3D11Texture2D* BackBuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
		assert(SUCCEEDED(hr));

		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
		assert(SUCCEEDED(hr));
		BackBuffer->Release();

		DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	}
	//Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0 };

		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Width;
		viewport.Height = Height;

		DeviceContext->RSSetViewports(1, &viewport);
	}

	

}

void DeleteBackBuffer()
{
	SAFE_RELEASE(RTV);
}
