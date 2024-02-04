#pragma once
#include "Object.h"

class Sprite : public Object
{
public:
	Sprite(wstring imgFile, wstring shaderFile = Texture_Shader);
	Sprite(wstring imgFile, float End_X, float End_Y, wstring shaderFile = Texture_Shader);
	Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y, wstring shaderFile = Texture_Shader);
	virtual void CreateBuffer(wstring imgFile, wstring shaderFile) ;


	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	//virtual Hit CheckRayCollision(Ray& ray) { return Hit(); }// ray 이벤트 미완성

	
	void Init_Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y); // startX ,startY EndX, EndY
public:
	wstring img;

	typedef struct OBB_Desc
	{
		D3DXVECTOR3 position;

		D3DXVECTOR3 Obb_up;
		D3DXVECTOR3 Obb_rigth;
		float length_Up;
		float length_rigth;
	};
	// sprite 를 가진 객체에서 호출해야되기에 static을 붙임
	static void Create_OBB(OBB_Desc& out, const D3DXMATRIX& world);
	static bool OBB( shared_ptr<class Sprite> a,  shared_ptr<class Sprite> b);
	static float SeperateAxis(D3DXVECTOR3& seperate, D3DXVECTOR3& e1, D3DXVECTOR3& e2);
	static bool Check_OBB(OBB_Desc obbA, OBB_Desc obbB);

private:
	

	D3DXMATRIX world;
	Vertex vertices[6];

	D3DXVECTOR3 TextSize;

	//Bound Shader
	Shader* BoundShader;
	ID3D11Buffer* BoundBuffer;
	struct BoundVertex
	{
		D3DXVECTOR3 postion;
	};
	virtual void CreateBoundBuffer(wstring shaderFile);

};

class Sprite_Manager
{
	friend class Sprite;
public:
	static ID3D11ShaderResourceView* Load(wstring img);
	static void ReMove(wstring img);

	struct Sprite_Desc
	{
	public:
		UINT RefCount = 0;
		ID3D11ShaderResourceView* srv = nullptr;
	};
	static map<wstring, Sprite_Desc> m;
};