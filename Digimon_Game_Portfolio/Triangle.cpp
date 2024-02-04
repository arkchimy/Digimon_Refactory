#include "stdafx.h"
#include "Triangle.h"
#include "Hit.h"
#include "Ray.h"

using namespace glm;

Triangle::Triangle(wstring shaderFile)
	:v0(vec3(-0.5f, -0.5f, 0.0f)), v1(vec3(-0.5f, +0.5f, 0.0f)), v2(+0.5f, -0.5f, 0.0f)
{
	CreateBuffer(shaderFile);
	UpdateWorld();
	Color(color);
}
Triangle::Triangle(wstring shaderFile,vec3 v0,vec3 v1, vec3 v2)
	:v0(v0),v1(v1),v2(v2)
{
	CreateBuffer(shaderFile);
	UpdateWorld();
	Color(color);
}

Triangle::~Triangle()
{
}



void Triangle::Update()
{
}

void Triangle::Render()
{
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 3);
}

void Triangle::UpdateWorld()
{

	D3DXMATRIX W, S, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	vec3 screen = ScreenCoordinate();

	D3DXMatrixTranslation(&T, screen.x, screen.y, screen.z);

	W = S * T;

	shader->AsMatrix("World")->SetMatrix(W);

}

vec3 Triangle::ScreenCoordinate()
{
	float aspect = float(Width) / Height;
	float scaleX = 2.f / Width * aspect;
	float scaleY = 2.f / Height;

	return vec3(position.x , position.y , position.z);
	return vec3(position.x * scaleX - aspect , position.y * scaleY - 1,position.z);
}

void Triangle::CreateBuffer(wstring shaderFile)
{
	shader = new Shader(shaderFile);


	vec3 vertices[3];
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(vec3) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

Hit Triangle::CheckRayCollision(Ray& ray)
{
	Hit hit = Hit{ -1.0, vec3(0.0), vec3(0.0) };

	vec3 point, faceNormal;
	float t, u, v;
	if (IntersectRayTriangle(ray.start, ray.dir, this->v0, this->v1,
		this->v2, point, faceNormal, t, u, v))
	{
		hit.d = t;
		hit.point = point; // ray.start + ray.dir * t;
		hit.normal = faceNormal;

		
	}

	return hit;
}

bool Triangle::IntersectRayTriangle(const vec3& orig, const vec3& dir, const vec3& v0, const vec3& v1, const vec3& v2, vec3& point, vec3& faceNormal, float& t, float& u, float& v)
{
	
	faceNormal = normalize(cross(v1 - v0, v2 - v0));

	if (dot(-dir, faceNormal) < 0.0f)
		return false;


	if (abs(dot(dir, faceNormal)) < 1e-2f)
		return false; // t 계산시 0으로 나누기 방지


	t = (dot(v0, faceNormal) - dot(orig, faceNormal))/
		(dot(dir, faceNormal));


	if (t < 0.0f)
		return false;

	point = orig + t * dir; // 충돌점
	const vec3 normal0 = normalize(cross(point - v2, v1 - v2));
	const vec3 normal1 = normalize(cross(point - v0, v2 - v0));
	const vec3 normal2 = normalize(cross(v1 - v0, point - v0));


	if (dot(normal0, faceNormal) < 0.0f)
		return false;
	if (dot(normal1, faceNormal) < 0.0f)
		return false;
	if (dot(normal2, faceNormal) < 0.0f)
		return false;

	// Barycentric coordinates 계산
	// 텍스춰링(texturing)에서 사용
	// u = ...
	// v = ...

	return true;

}
