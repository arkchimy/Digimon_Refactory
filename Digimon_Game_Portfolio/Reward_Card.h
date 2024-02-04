#pragma once
class Reward_Card
{
public:
	Reward_Card(wstring Cardfile,const wstring ShaderFile = Card_Shader);
	~Reward_Card();
public:
	void Update();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
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
};

class Card_Manager
{
	friend Reward_Card;

public:
	static void Create(vector<wstring> data);
	static shared_ptr<Reward_Card> Load();
	static shared_ptr<class Digimon> FindDigimon(wstring target);
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static void Use_Card();
	static void Init_Visible();

	static queue<shared_ptr<Reward_Card>> q;

	
};

