#pragma once
class Reward_Card
{
public:
	Reward_Card(wstring Cardfile,const wstring ShaderFile = Card_Shader);
	Reward_Card();
	~Reward_Card();
public:
	void Update();
	//void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P) {};
	void Render();
	void Scale(D3DXVECTOR3 val);
	void Position(D3DXVECTOR3 val);
	void Visible(bool val);
	void Use_Card();
	shared_ptr<class Digimon> ReplaceDigimon();

	void Set_Owner(class Stage* owner) { Owner = owner; }

	bool MouseOver() { return bOver; }
	void MouseOver(bool val) { bOver = val; }
	wstring cardname;
private:
	unique_ptr<class Animation> sprites;
	queue<shared_ptr<class Digimon>> q;
	shared_ptr<class Digimon> digimon;

	class Stage* Owner;
	bool bOver = false;

	/*  ∏Æ∆—≈‰∏µ  */
public:
	void Load();
	shared_ptr<class Digimon> CardDigimon() { return digimon; }
	shared_ptr<class Animation> m_animation;

private:
	D3DXVECTOR3 m_position = { 0,0,0 };
	D3DXVECTOR3 m_rotator = { 0,0,0 };
	D3DXVECTOR3 m_scale = { 1,1,1 };
	
};

class Card_Manager
{
	friend Reward_Card;

public:
	static void Create(vector<wstring> data);
	//static shared_ptr<class Digimon> FindDigimon(wstring target);
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static void Use_Card();
	static void Init_Visible();

	static queue<shared_ptr<Reward_Card>> q;

	// ∏Æ∆—≈‰∏µ
	static vector<class Shader*> shader_vec;
	static vector<shared_ptr<class Animation>> animations;


	static vector<ID3D11ShaderResourceView*> srv_vec;
	static vector<vector<ID3D11Buffer*>> buffer_vec;

	static shared_ptr<class Animation> Load(int& cardID);
	
	static int shader_idx;
	
};

