#pragma once
#define �Ʊ� 0
#define ���� 1

enum class Bullet_Type 
{
	Normal = 1 << 0,
	through = 1 << 1,
	explosion = 1 << 2,
	Spread =  1 << 3
};
class Bullet
{
public:
	Bullet();
	Bullet(UINT bullet_type);
	~Bullet();


	void Init_Info(wstring ImgFile, float cnt, float bullet_speed, float start);
	void Init_Info(Sprite_Info info, float bullet_speed, wstring effect_File, float knock_power = -1.f);
	void Init_Info(Sprite_Info info, float bullet_speed, vector<D3DXVECTOR4> pos_vec, wstring effect_File, float knock_power = -1.f);

	void Init_Info(wstring ImgFile, float cnt, float bullet_speed, float width , float height, float knock_power = -1.f);
	void Init_Info(wstring ImgFile, float cnt, float bullet_speed, vector<D3DXVECTOR4> pos_vec, float knock_power = -1.f);
	
	void Fire(D3DXVECTOR2 targetpos);
	void Update(); //  bullet Speed �ʿ�
	void Render();

	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	// =========== Bullet ==========
	void Scale(D3DXVECTOR3 val);    // ������ ����
	void Position(D3DXVECTOR3 val); // ó���� �߻�ü pos ���� ������
	void Rotator(D3DXVECTOR3 val);
	void Set_Dir(D3DXVECTOR3 val) { bvisible = true;  Dir = val * speed; }

	bool Visible() { return bvisible; }
	void Team_ID(UINT val) { team_ID = val; }
	UINT team_ID;

	float Power() { return power; }
	float Knock_Back() { return knock_back; }
	UINT Bullet_Type() { return type; }
	//�浹üũ
	// ������ �����ߴ���.

private:
	vector<shared_ptr<class Sprite>> sprites;
	wstring Effect_File;
	D3DXVECTOR3 explosion_Pos;
	D3DXVECTOR3 Dir;
	
	float power  = 1.f;
	float speed;

	float radius = 300.f; // ���� �ݰ濡 ����.
	
	int current_idx = 0;
	int idx_size = 0; 
	bool bvisible;
	float knock_back;

	const UINT type;
	vector<shared_ptr<class Enemy>> Targetted_Actor;
	
	

};

class Bullet_Manager
{
	//Single torn
	friend class Bullet;

public:
	static void Create(int idx);
	//static vector<shared_ptr<Bullet>> Load(int cnt);
	static shared_ptr<Bullet> Load(wstring imgfile);

	static void Render();
	static void Update();
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

private:
	static queue<shared_ptr<Bullet>> bullets;
	static vector<shared_ptr<Bullet>> bullets_vec; // ��� Bullet
	static map<wstring, queue<shared_ptr<Bullet>>> m;
};

class Effect_Manager
{
	//Single torn
	friend class Bullet;

public:
	static void Create(int idx);
	//static vector<shared_ptr<Bullet>> Load(int cnt);
	static shared_ptr<class Animation> Load(wstring imgfile);

	static void Render();
	static void Update();
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static void Level_Up(D3DXVECTOR3 pos);
private:

	static map<wstring, queue<shared_ptr<class Animation>>> m;
	static shared_ptr<class Animation> level_up;
	
};

