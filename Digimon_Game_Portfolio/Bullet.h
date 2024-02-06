#pragma once
#define 아군 0
#define 적군 1

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


	//void Init_Info(wstring ImgFile, float cnt, float bullet_speed, float start);
	void Init_Info(Sprite_Info info, float bullet_speed, wstring effect_File, float knock_power = -1.f);
	void Init_Info(Sprite_Info info, float bullet_speed, vector<D3DXVECTOR4> pos_vec, wstring effect_File, float knock_power = -1.f);

	//void Init_Info(wstring ImgFile, float cnt, float bullet_speed, float width , float height, float knock_power = -1.f);
	//void Init_Info(wstring ImgFile, float cnt, float bullet_speed, vector<D3DXVECTOR4> pos_vec, float knock_power = -1.f);
	
	void CreateShaderAndBuffer();

	void Fire(D3DXVECTOR2 targetpos);
	void Update(); //  bullet Speed 필요
	void Render();

	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	// =========== Bullet ==========
	void Scale(D3DXVECTOR3 val);    // 스케일 조정
	void Position(D3DXVECTOR3 val); // 처음에 발사체 pos 에서 나가기
	void Rotator(D3DXVECTOR3 val);
	void Set_Dir(D3DXVECTOR3 val) { bvisible = true;  m_Dir = val * m_Speed; }

	bool Visible() { return bvisible; }
	void Team_ID(UINT val) { team_ID = val; }
	UINT team_ID;

	float Power() { return power; }
	float Knock_Back() { return knock_back; }
	UINT Bullet_Type() { return type; }
	//충돌체크
	// 목적지 도착했는지.

private:
	//vector<shared_ptr<class Sprite>> sprites;

	// Hit 관련
	wstring Effect_File;
	float power = 1.f;
	float radius = 300.f; // 폭발 반경에 쓰임.
	float knock_back;

	// 중복 Hit방지
	vector<shared_ptr<class Enemy>> Targetted_Actor;

	// 이동 관련
	D3DXVECTOR3 explosion_Pos;// 폭발하는 위치
	D3DXVECTOR3 m_Dir;// 총알의 진행방향
	float m_Speed;

	// 애니메이션
	int current_idx = 0;
	int m_SpriteSize = 0; 
	float m_localTime = 0.f; // delta time 을 누적함
	// State 
	bool bvisible;
	const UINT type;
	
	//   리팩토링

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Rotator;

	class Shader* m_Shader;
	vector<ID3D11Buffer*> buffer_vec;
	vector<ID3D11ShaderResourceView*> srv_vec;
	
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
	static vector<shared_ptr<Bullet>> bullets_vec; // 모든 Bullet
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

