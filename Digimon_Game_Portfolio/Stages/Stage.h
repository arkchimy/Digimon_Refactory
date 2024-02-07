#pragma once

class Stage 
{
public:
	Stage();
	Stage(vector<wstring> imagefiles,vector<wstring> shaderfiles);
	virtual ~Stage();

	virtual void Init_Stage(shared_ptr<class Player> player) = 0;
	virtual void Update() = 0;
	virtual void Render();


	void CreateShaderAndBuffer(vector<wstring> layerfile, vector<wstring> shaderfiles);
	//void CreateShaderAndBuffer();
	virtual void Battle_Stage() {};
	void Battle(bool val);
	virtual void Stage_Clear(); // 
	virtual void Wave_Clear() {}; //UI 갱신목적

	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	void UpdateWorld();
	
	virtual void Take_Damage(class Bullet* causer) ;
	virtual void Add_Digimon(shared_ptr<class Digimon> data);

	void SetPlayer(shared_ptr<class Player> input) { Player = input; }

	vector<shared_ptr<class Enemy>> Enemy_vec;
	queue<shared_ptr<class Enemy>> Enemy_pool;

	// 리팩토링
	virtual void ClickEvent() {};
protected:
	class CastleGate* Gate;

	shared_ptr<class Player> Player;
	int wave = 0;// 몇번쨰 Wave인지

	vector<unique_ptr<Shader>> shader_vec;
	

	vector<D3DXVECTOR3> position;
	vector<D3DXVECTOR3> scale;
	vector<D3DXVECTOR3> rotator;


	vector<ID3D11Buffer*> buffer_vec;
	vector<ID3D11ShaderResourceView*> srv_vec;

	//map layer object들
};

