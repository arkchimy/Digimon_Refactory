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


	float sizeX = (End_X > 0) ? End_X * (float)info.Width : (float)info.Width;
	sizeX -= start_X * (float)info.Width;

	float sizeY = (End_Y > 0) ? End_Y * (float)info.Height : (float)info.Height;
	sizeY -= start_Y * (float)info.Height;

	TextSize = D3DXVECTOR3(sizeX, sizeY,1);

	scale.x *= TextSize.x;
	scale.y *= TextSize.y;
}
void Sprite::Create_OBB(OBB_Desc& out, const D3DXMATRIX& world)
{
	out.position = D3DXVECTOR3{ world._41,world._42 ,world._43 };
	out.Obb_rigth = D3DXVECTOR3{ world._11,world._12 ,world._13 };
	out.Obb_up = D3DXVECTOR3{ world._21,world._22 ,world._23 };

	D3DXVec3Normalize(&out.Obb_rigth, &out.Obb_rigth);
	D3DXVec3Normalize(&out.Obb_up, &out.Obb_up);

	out.length_rigth = world._11 * Collider_Length;
	out.length_Up = world._22 * Collider_Length;
}
bool Sprite::OBB( shared_ptr<class Sprite> a,  shared_ptr<class Sprite> b)
{
	return AABB(a, b);
	OBB_Desc obbA, obbB;
	
	float xScale, yScale;

	Create_OBB(obbA, a->world);

	Create_OBB(obbB, b->world);

	return Check_OBB(obbA, obbB);
	
}
bool Sprite::AABB(shared_ptr<class Sprite> a, shared_ptr<class Sprite> b)
{
	// OBB의 속도 개선을 위해 단순히 거리로 체크하는 방식 구현해봄
	D3DXVECTOR3 distance = a->Position() - b->Position();
	float len = D3DXVec3Length(&distance);

	return len <= 50.f ? true : false;

}
bool Sprite::AABB(const D3DXVECTOR3 a, const D3DXVECTOR3 b)
{
	D3DXVECTOR3 distance = a - b;
	float len = D3DXVec3Length(&distance);

	return len <= 50.f ? true : false;
}


float Sprite::SeperateAxis(D3DXVECTOR3& seperate, D3DXVECTOR3& e1, D3DXVECTOR3& e2)
{
	float r1 = fabsf(D3DXVec3Dot(&seperate, &e1));
	float r2 = fabsf(D3DXVec3Dot(&seperate, &e2));

	return r1 + r2;
}

bool Sprite::Check_OBB(OBB_Desc obbA, OBB_Desc obbB)
{
	D3DXVECTOR3 Up_A = obbA.Obb_up, ea1 = Up_A * obbA.length_Up;
	D3DXVECTOR3 R_A = obbA.Obb_rigth, ea2 = R_A * obbA.length_rigth;

	D3DXVECTOR3 Up_B = obbB.Obb_up, eb1 = Up_B * obbB.length_Up;
	D3DXVECTOR3 R_B = obbB.Obb_rigth, eb2 = R_B * obbB.length_rigth;

	D3DXVECTOR3 direction = obbA.position - obbB.position;


	float lengthA = 0.0f, lengthB = 0.0f, length = 0.0f;

	lengthA = D3DXVec3Length(&ea1);
	lengthB = SeperateAxis(Up_A, eb1, eb2);
	length = fabsf(D3DXVec3Dot(&direction, &Up_A));
	if (length > lengthA + lengthB)
		return false;

	lengthA = D3DXVec3Length(&ea2);
	lengthB = SeperateAxis(R_A, eb1, eb2);
	length = fabsf(D3DXVec3Dot(&direction, &R_A));
	if (length > lengthA + lengthB)
		return false;


	lengthA = SeperateAxis(Up_B, ea1, ea2);
	lengthB = D3DXVec3Length(&eb1);
	length = fabsf(D3DXVec3Dot(&direction, &Up_B));
	if (length > lengthA + lengthB)
		return false;

	lengthA = SeperateAxis(R_B, ea1, ea2);
	lengthB = D3DXVec3Length(&eb2);
	length = fabsf(D3DXVec3Dot(&direction, &R_B));
	if (length > lengthA + lengthB)
		return false;

	return true;
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


