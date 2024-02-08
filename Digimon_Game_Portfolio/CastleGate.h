#pragma once
#include "Actors/ICombat.h"
class CastleGate :public ICombat
{
public:
	CastleGate();
	~CastleGate();
	void Render();
	void Update();
	void Animation();

	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir); // 누가 데미지를 줬는지 중요한가?
	virtual void Fire() {};
	virtual bool IsDeathMode();


	vector<D3DXVECTOR3> Slot_Position();
protected:
	vector<D3DXVECTOR3> slot_position;


	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotator;

	unique_ptr<class Shader> shader;
	ID3D11ShaderResourceView* srv;
	vector<ID3D11Buffer*> buffer;

	class unique_ptr<class Sprite> Castle;

	float hp =10;
};

