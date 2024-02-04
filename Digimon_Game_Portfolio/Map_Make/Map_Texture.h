#pragma once


class Map_Texture 
{
public:
	Map_Texture(wstring imgFile , wstring shaderFile = Map_Texture_Shader);
	~Map_Texture();

	void Update();
	void Render();
	
	void Position(D3DXVECTOR3 val);
	void Scale(D3DXVECTOR3 val);

	void UpdateWorld() ; // 위치 스케일 변화 등
	void Animation(D3DXVECTOR3 speed);

	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	void Move_BackPosition(D3DXVECTOR3);
	void ScreenRange_OutChk();
private:
	
	unique_ptr<class Sprite> Map;
	unique_ptr<class Sprite> Swap_Map;

	D3DXVECTOR3 Map_Pos;
	D3DXVECTOR3 Swap_Map_Pos;
};
