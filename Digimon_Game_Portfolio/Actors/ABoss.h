#pragma once
#include "ICombat.h"
#include "Enemy.h"
#define BossQue_SIZE 1
class ABoss : public Enemy, ICombat
{
public:
	ABoss(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);
	ABoss(wstring imgfile, float width, float height, Sprites_Info info, int level);
	~ABoss();
public:
	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir);
	virtual void Fire();
	virtual bool IsDeathMode();
	
	virtual void Update() override;
	virtual void Respawn();
	D3DXVECTOR2 speed = {90,0};
};