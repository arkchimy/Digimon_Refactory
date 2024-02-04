#pragma once
#include "Stages/Stage.h"
#define Enemy_Cnt 5
#define Wave_SIZE 7
#define Enemy_PoolSize 20
#define REWARD_SIZE 2
class Stage_01 :public Stage
{
public:
	Stage_01();
	~Stage_01();
	virtual void Init_Stage(shared_ptr<class Player> player);
	virtual void Battle_Stage();
	void init_Wave();
	void Boss_Appear();

	void Update() override;
	void Render() override;
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)override;
	virtual void Wave_Reward();
	virtual void Wave_Clear() override;

	virtual void Add_Digimon(shared_ptr<class Digimon> data) override;

private:
	vector<shared_ptr<class Reward_Card>> reward;
	queue<shared_ptr<class ABoss>> boss_que;
	shared_ptr<class ABoss> boss;


	bool bclear;
};

