#pragma once
#include "Attacker.h"
class Enemy : public Attacker
{
public:
	Enemy(wstring imgfile, float width, float height, Sprites_Info info, int level);
	Enemy(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);
	~Enemy();


	//virtual void Animation();

	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir = {+1,0,0}) override;// 추후에 기능추가
	virtual void Update() override;
	virtual void Render() override;

	virtual void FindLookAtTarget() {};

	void Battle(bool val);
	void Stage_Clear();
	virtual void Respawn();

	void Fire();
public:
	float distance = -340.f; // 어느지점까지 움직이느냐. Distance까지 움직인후
	bool bTurn = false;


	// 리팩토링
	void Init_info();

};


class Enemy_Manager
{
public:
	static void CreatePooling(int pool);
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static shared_ptr<Enemy> Load(wstring imgfile);

private:
	static vector<shared_ptr<Enemy>> m;
	static int idx;

};

