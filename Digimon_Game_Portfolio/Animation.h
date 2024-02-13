#pragma once
#include "Sprite_info.h"

enum class PlayMode
{
	Loop = 0,End,Reverse,End_Stop,
};
class Animation
{
public:
	Animation(vector<shared_ptr<class Sprite>> , PlayMode mode = PlayMode::Loop);
	Animation(Sprite_Info info, PlayMode mode = PlayMode::Loop);
	Animation(class Shader* shader, ComPtr<ID3D11ShaderResourceView> srv_vec, vector<ComPtr<ID3D11Buffer>> buffer_vec, PlayMode mode = PlayMode::Loop);

	void Update();
	void Render();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	void UpdateWorld();
	void UpdateSrvAndBuffer(ComPtr<ID3D11ShaderResourceView> srv, vector<ComPtr<ID3D11Buffer>> buffer_vec);


	void Start() { bvisible = true; index = 0; playtime = 0.f; }
	void Set_Owner(class Attacker* owner) { Owner = owner; }

	void Position(D3DXVECTOR3 val);
	void Scale(D3DXVECTOR3 val);

	D3DXVECTOR3 Position();
	D3DXVECTOR3 Scale();
	int Current_Idx() { return index; }
	bool Visible() { return bvisible; } // Effect�� ����ϴ� �뵵
	void Visible(bool val) { bvisible = val; }
	void PlaySpeed(float val) { play_rate += val; }
private:
	PlayMode mode;
	vector<shared_ptr<class Sprite>> sprites_vec;

	float playtime;
	int index;
	float play_rate = 1.f;
	bool bvisible = false; // Effect_�� ����ϴ� �뵵

	class Attacker* Owner = nullptr;

private:
	class Shader* m_Shader =  nullptr;
	vector<ComPtr<ID3D11Buffer>> m_buffer_vec;
	ComPtr<ID3D11ShaderResourceView> m_srv;

	vector<unique_ptr<class Animation>> animations;

	D3DXVECTOR3 m_position = { 0,0,0 };
	D3DXVECTOR3 m_rotator = { 0,0,0 };
	D3DXVECTOR3 m_scale = { 1,1,1 };

};

