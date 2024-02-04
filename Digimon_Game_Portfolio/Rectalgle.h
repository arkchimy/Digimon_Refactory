#pragma once
#include "Object.h"
class Rectalgle : public Object
{
public: 
	Rectalgle(wstring imgFile, wstring shaderfile = Player_Shader);
	Rectalgle();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	void Color(D3DXCOLOR vec) override {};
	virtual void Render();
public:
	virtual void CreateBuffer(wstring imgFile, wstring shaderFile);
	virtual void UpdateWorld();
	
	
	Vertex vertices[6];
};

