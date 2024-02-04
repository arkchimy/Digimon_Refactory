#pragma once
class Decal
{
public:
	Decal();
	~Decal();
public:
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	void Update();
	void Render();

	void Scale(D3DXVECTOR3 val);
	void Position(D3DXVECTOR3 val);

	void Visible(bool val) { bVisible = val; }
	bool Visible() { return bVisible; }

private:
	unique_ptr<class Sprite> sprite;
	bool bVisible;
};

