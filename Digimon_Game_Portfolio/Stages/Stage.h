#pragma once

class Stage 
{
public:
	Stage();

	virtual void Init_Stage(shared_ptr<class Player> player) = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Battle_Stage() {};
	void Battle(bool val);
	virtual void Stage_Clear(); // 
	virtual void Wave_Clear(); //UI 갱신목적

	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	
	virtual void Take_Damage(class Bullet* causer) ;
	virtual void Add_Digimon(shared_ptr<class Digimon> data);

	void SetPlayer(shared_ptr<class Player> input) { Player = input; }

	vector<shared_ptr<class Enemy>> Enemy_vec;
	queue<shared_ptr<class Enemy>> Enemy_pool;


protected:
	class CastleGate* Gate;
	Layers layers;
	Grounds ground;

	shared_ptr<class Player> Player;
	int wave = 0;// 몇번쨰 Wave인지
	//map layer object들
};

