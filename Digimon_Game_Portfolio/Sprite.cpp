#include "stdafx.h"
#include "Sprite.h"
#define Collider_Length 0.3

extern bool bDrawBound;
Sprite::Sprite(wstring imgFile, wstring shaderFile)
	:Sprite(imgFile, 0.f, 0.f, 1.f, 1.f, shaderFile)
{

}

Sprite::Sprite(wstring imgFile,float End_X,float End_Y, wstring shaderFile)
	:Sprite(imgFile, 1.f, 1.f, End_X, End_Y, shaderFile)
{

}

Sprite::Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y, wstring shaderFile)
{
	Init_Sprite(imgFile, start_X, start_Y, End_X, End_Y);
	CreateBuffer(imgFile, shaderFile);
	CreateBoundBuffer(Shaders + L"014_Bounding.fx");

}


Sprite::Sprite(vector<ID3D11Buffer*>& ownerbuffer, wstring imgFile, wstring shaderFile)
	:Sprite(ownerbuffer, imgFile, 0, 0, 1, 1, shaderFile)
{
}

Sprite::Sprite(vector<ID3D11Buffer*>& ownerbuffer, wstring imgFile, float start_X, float start_Y, float End_X, float End_Y, wstring shaderFile)
{
	Init_Sprite(imgFile, start_X, start_Y, End_X, End_Y);

	CreateBuffer(imgFile, shaderFile);
	ownerbuffer.push_back(vertexBuffer);
	CreateBoundBuffer(Shaders + L"014_Bounding.fx");

}


void Sprite::Init_Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y)
{

	D3DXMatrixIdentity(&world);// world 초기화

	D3DX11_IMAGE_INFO info;
	D3DX11GetImageInfoFromFile(imgFile.c_str(), NULL, &info, NULL);

	D3DXVECTOR3 TextSize;

	TextSize.x = float(info.Width);
	TextSize.y = float(info.Height);
	TextSize.z = 1.f;

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	
	start_X = (start_X > 0) ? start_X / TextSize.x : 0.f;
	start_Y = (start_Y > 0) ? start_Y / TextSize.y : 0.f;
	End_X = (End_X > 1.f) ?  End_X / TextSize.x : 1.f  ;
	End_Y = (End_Y > 1.f) ?  End_Y / TextSize.y : 1.f ;

	vertices[0].Uv = D3DXVECTOR2(start_X, End_Y);
	vertices[1].Uv = D3DXVECTOR2(start_X, start_Y);
	vertices[2].Uv = D3DXVECTOR2(End_X, End_Y);
	vertices[3].Uv = D3DXVECTOR2(End_X, End_Y);
	vertices[4].Uv = D3DXVECTOR2(start_X, start_Y);
	vertices[5].Uv = D3DXVECTOR2(End_X, start_Y);



}

bool Sprite::AABB(const D3DXVECTOR3 a, const D3DXVECTOR3 b)
{
	D3DXVECTOR3 distance = a - b;
	float len = D3DXVec3Length(&distance);

	return len <= 50.f ? true : false;
}


void Sprite::CreateBoundBuffer(wstring shaderFile)
{
	BoundShader = new Shader(shaderFile);
	BoundVertex verties[5];

	verties[0].postion = D3DXVECTOR3{ -0.5f, -0.5f, 0.f };
	verties[1].postion = D3DXVECTOR3{ -0.5f, +0.5f, 0.f };
	verties[2].postion = D3DXVECTOR3{ +0.5f, +0.5f, 0.f };
	verties[3].postion = D3DXVECTOR3{ +0.5f, -0.5f, 0.f };
	verties[4].postion = D3DXVECTOR3{ -0.5f, -0.5f, 0.f };

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoundVertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = verties;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &BoundBuffer);
		assert(SUCCEEDED(hr));
	}


}
void Sprite::CreateBuffer(wstring imgFile, wstring shaderFile)
{

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
	
}


///
//   
/// 


map<wstring, Sprite_Manager::Sprite_Desc> Sprite_Manager::m;
map < wstring, vector<ID3D11Buffer*>> Sprite_Manager::m_buffer;
map < wstring, vector<vector<ID3D11Buffer*>>> Sprite_Manager::m_buffer_vec;

ID3D11ShaderResourceView* Sprite_Manager::Load(wstring imgFile)
{
	UINT cnt = m.count(imgFile);
	if (cnt == 0)
	{	// 해당 이미지의 srv가 없다면 생성
		m[imgFile].RefCount++;
		//Create SRV
		{
			HRESULT hr = D3DX11CreateShaderResourceViewFromFile
			(
				Device, imgFile.c_str(), NULL, NULL, &m[imgFile].srv, NULL
			);
			assert(SUCCEEDED(hr));
		}
		
	}
	return m[imgFile].srv;
}
vector<ID3D11Buffer*> Sprite_Manager::LoadBuffer(wstring imgfile)
{
	UINT cnt = m_buffer.count(imgfile);
	if (cnt != 0)
		return m_buffer[imgfile];

	return vector<ID3D11Buffer*>(); 
}

vector<vector<ID3D11Buffer*>> Sprite_Manager::LoadBufferVector(wstring imgfile)
{
	UINT cnt = m_buffer_vec.count(imgfile);
	if (cnt != 0)
		return m_buffer_vec[imgfile];

	return vector<vector<ID3D11Buffer*>>();
}

void Sprite_Manager::StoreBuffer(wstring imgfile, vector<ID3D11Buffer*> buffer_vec)
{
	UINT cnt = m_buffer.count(imgfile);
	if (cnt == 0)
	{
		m_buffer[imgfile] = buffer_vec;
	}
}

void Sprite_Manager::StoreBufferVector(wstring imgfile, vector<vector<ID3D11Buffer*>> buffer_vec)
{
	UINT cnt = m_buffer_vec.count(imgfile);
	if (cnt == 0)
	{
		m_buffer_vec[imgfile] = buffer_vec;
	}
}



void Sprite_Manager::ReMove(wstring imgFile)
{
	UINT cnt = m.count(imgFile);
	if (cnt != 0)
	{	// 해당 이미지의 srv가 없다면 생성
		m[imgFile].RefCount--;
		CheckFalse(m[imgFile].RefCount == 0);
		SAFE_RELEASE(m[imgFile].srv);
	}
}


