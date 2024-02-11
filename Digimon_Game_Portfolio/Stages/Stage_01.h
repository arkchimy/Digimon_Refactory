#pragma once
#include "Stages/Stage.h"
#define Enemy_Cnt 5
#define Wave_SIZE 7



class Stage_01 :public Stage
{
public:
	Stage_01();
	virtual ~Stage_01();
	virtual void Init_Stage(shared_ptr<class Player> player);
	virtual void Battle_Stage();
	void init_Wave();
	void Boss_Appear();

	virtual void ClickEvent() override;

	virtual void Update() override;
	virtual void Render() override;

	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)override;
	virtual void Wave_Reward();
	virtual void Wave_Clear() override;

	virtual void Add_Digimon(shared_ptr<class Digimon> data) override;
	virtual void ReturnSlot(D3DXVECTOR3 slotpos);

private:
	//vector<shared_ptr<class Reward_Card>> reward;
	queue<shared_ptr<class ABoss>> boss_que;
	shared_ptr<class ABoss> boss;


	// sampler 테스트
	ID3D11SamplerState* m_samplerState;

	void CreateSamplerState();


	bool bclear =false;
	//리팩토링
	vector<unique_ptr<class Reward_Card>> reward;



	//리팩토링


};

