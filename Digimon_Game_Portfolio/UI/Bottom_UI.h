#pragma once
class Bottom_UI
{

public:
	Bottom_UI(wstring file);
	~Bottom_UI();

	void Update();
	void Render();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	
	void Scael(D3DXVECTOR3 val);
	void Position(D3DXVECTOR3 val);
	
	class Sprite* sprite;
	class Sprite* victory;

	static void GameClear(bool val) { bvictory = val; }
private:
	static bool bvictory;
};