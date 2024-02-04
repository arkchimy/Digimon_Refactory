#include "stdafx.h"
#include "Rectalgle.h"

Rectalgle::Rectalgle(wstring imgFile, wstring shaderfile)
{
	CreateBuffer(imgFile, shaderfile);
	UpdateWorld();
	Color(color);
}

Rectalgle::Rectalgle()
{
}



void Rectalgle::CreateBuffer(wstring imgFile ,wstring shaderFile)
{
	shader = new Shader(shaderFile);

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);

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
	//Create SRV
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			Device, imgFile.c_str(), NULL, NULL, &srv, NULL
		);
		assert(SUCCEEDED(hr));
	}
	shader->AsShaderResource("Map")->SetResource(srv);
}

void Rectalgle::UpdateWorld()
{
	D3DXMATRIX W, S, T;

	D3DXMatrixScaling(&S, scale.x, scale.y,scale.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	W = S * T;

	shader->AsMatrix("World")->SetMatrix(W);
}
void Rectalgle::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);
}

void Rectalgle::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
}
