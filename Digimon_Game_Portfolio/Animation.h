#pragma once
enum class PlayMode
{
	Loop = 0,End,Reverse,End_Stop,
};
class Animation
{
public:
	Animation(vector<shared_ptr<class Sprite>> , PlayMode mode = PlayMode::Loop);
	Animation(Sprite_Info info, PlayMode mode = PlayMode::Loop);
	void Update();
	void Render();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	void UpdateWorld();

	void Start() { bvisible = true; index = 0; playtime = 0.f; }

	void Set_Owner(class Attacker* owner) { Owner = owner; }

	void Position(D3DXVECTOR3 val);
	void Scale(D3DXVECTOR3 val);

	D3DXVECTOR3 Position();
	D3DXVECTOR3 Scale();
	int Current_Idx() { return index; }
	bool Visible() { return bvisible; } // Effect를 사용하는 용도
	void Visible(bool val) { bvisible = val; }
	void PlaySpeed(float val) { play_rate += val; }
private:
	PlayMode mode;
	vector<shared_ptr<class Sprite>> sprites_vec;
	float playtime;
	int index;
	float play_rate;
	bool bvisible = false; // Effect_를 사용하는 용도

	class Attacker* Owner = nullptr;
};

