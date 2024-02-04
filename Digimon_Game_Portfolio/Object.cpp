#include "stdafx.h"
#include "Object.h"


void Object::Position(D3DXVECTOR3 vec)
{
	position = vec;
	UpdateWorld();
}


void Object::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);
}

void Object::Position(float x, float y, float z)
{
	Position(D3DXVECTOR3(x, y, z));

}

void Object::Scale(D3DXVECTOR3 vec)
{
	scale = vec;
	UpdateWorld();
}

void Object::Rotator(float x, float y, float z)
{
	Rotator({ x,y,z });
}

void Object::Rotator(D3DXVECTOR3 vec)
{
	rotator = vec;
	UpdateWorld();
}


void Object::Scale(float x, float y, float z)
{
	Scale(D3DXVECTOR3(x, y, z));
}

void Object::Color(D3DXCOLOR vec)
{
	color = vec;
	shader->AsVector("Color")->SetFloatVector(color);
}

void Object::Color(float r, float g, float b)
{
	Color(D3DXCOLOR(r, g, b,1.f));
}


