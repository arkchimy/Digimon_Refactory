#pragma once
#include "Actors/Attacker.h"
class Digimon: public Attacker
{
public:
	//Digimon(wstring imgfile,float width, float height,float idle_size ,int level);
	Digimon(wstring imgfile, float width, float height, Sprites_Info info, int level);
	Digimon(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);
	~Digimon();

public:
	void Update() override;
	//virtual void Animation() override;
	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir = {-1,0,0}) override;
	void FindLookAtTarget();
	shared_ptr<class Enemy> SearchTarget();

	void Battle(bool val);
	void Stage_Clear();

	void Fire();
	void Animation_Start(int idx);

	void MoveTo(D3DXVECTOR3 goal);
	bool ClickEvent(D3DXVECTOR2 mouse);
	bool ClickEvent(D3DXVECTOR2 mouse,shared_ptr<class Digimon> drag_digimon);
	void Drag(bool val); //Drag 취소할 때
	shared_ptr<class Digimon> Evolution();

	D3DXVECTOR2 Skill_Target_Pos;
	D3DXVECTOR3 goal_pos;
private:
	D3DXVECTOR3 bullet_Dir;
	D3DXVECTOR3 before_pos;
	bool bMove = false;
	bool bDrag = false;
	int combine_cnt = 0;
	wstring next_digimon;
};


