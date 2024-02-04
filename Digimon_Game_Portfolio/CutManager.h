#pragma once

class CutScean
{
public:
	CutScean();
	~CutScean();
public:
	void Update();
	void Render();
	void Scale(D3DXVECTOR3 val);
	void Position(D3DXVECTOR3 val);

	void Cut_Start(wstring cutfile);
	void Cut_End();

	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	shared_ptr<class Animation> Scene;
	unique_ptr<class Animation> Scene_bg;
};

class CutManager
{
public:
	static void Create();
	static shared_ptr<class Animation> Load(wstring );
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static map<wstring, shared_ptr<class Animation>> m;
};

