#pragma once
#include "Rectalgle.h"

class Player 
{
public:
	Player(wstring ShaderFile = Player_Shader);
	~Player();

	void Update() ;
	void Render() ;
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P) ;
	void UpdateWorld() ;

	void Ready();
	void Battle(bool val);
	void Stage_Clear();

	void Slot_Set(vector<D3DXVECTOR3> slot_pos);

	void Use_Skill(int num, D3DXVECTOR2 MousePos);
	void Decal_Visible(int idx ,bool val);
	void ClickEvent();
	void ClickUp_Event();
	// 시간 측정

	void Add_Digimon(shared_ptr<class Digimon> input);
	vector<wstring> Player_Cards() { return cards; }
	

	int DigimonCnt() { return My_Digimon.size(); }
private:
	vector<shared_ptr<class Digimon>> My_Digimon;
	shared_ptr<class Digimon> drag_digimon = nullptr;
	vector<wstring> cards;
};

