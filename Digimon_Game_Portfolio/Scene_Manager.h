#pragma once

class Scene_Manager
{
public:
	Scene_Manager();
	static void Init_Stage();
	static void Start_Stage();
	static void StandBy_Stage(int stage_num);
	static void Stage_Complete(bool val);

	void Update();
	void Render();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	static void Take_Damage(class Bullet* causer);
	static vector<shared_ptr<class Enemy>> Get_Enemies();
	static std::shared_ptr<class Player> Player;
private:
	static std::unique_ptr<class Stage> Current_Scene;
	
};

