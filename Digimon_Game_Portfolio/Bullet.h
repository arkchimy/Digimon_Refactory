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

	void Init_Info(Sprite_Info info, float bullet_speed, wstring effect_File, float knock_power = -1.f);
	void Init_Info(Sprite_Info info, float bullet_speed, vector<D3DXVECTOR4> pos_vec, wstring effect_File, float knock_power = -1.f);
	
	void CreateShaderAndBuffer();

	void Fire(D3DXVECTOR2 targetpos);
	void Update(); //  bullet Speed �ʿ�
	void Render();

	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	// =========== Bullet ==========
	void Scale(D3DXVECTOR3 val);    // ������ ����
	void Position(D3DXVECTOR3 val); // ó���� �߻�ü pos ���� ������
	void Rotator(D3DXVECTOR3 val);
	void Set_Dir(D3DXVECTOR3 val) { bvisible = true;  m_Dir = val * m_Speed; }
	void Init_BulletType(const UINT& type);


	bool Visible() { return bvisible; }
	void Team_ID(UINT val) { team_ID = val; }
	UINT team_ID;

	float Power() { return power; }
	float Knock_Back() { return knock_back; }
	UINT Bullet_Type() { return type; }
	//�浹üũ
	// ������ �����ߴ���.

private:
	// Hit ����
	wstring Effect_File;
	float power = 1.f;
	float radius = 300.f; // ���� �ݰ濡 ����.
	float knock_back;

	// �ߺ� Hit����
	vector<shared_ptr<class Enemy>> Targetted_Actor;

	// �̵� ����
	D3DXVECTOR3 explosion_Pos;// �����ϴ� ��ġ
	D3DXVECTOR3 m_Dir;// �Ѿ��� �������
	float m_Speed;

	// �ִϸ��̼�
	int current_idx = 0;
	int m_SpriteSize = 0; 
	float m_localTime = 0.f; // delta time �� ������
	// State 
	bool bvisible;
	UINT type;
	
	//   �����丵

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Rotator;

	class Shader* m_Shader;
	vector<ID3D11Buffer*> buffer_vec;
	ID3D11ShaderResourceView* m_srv;
	
};

class Bullet_Manager
{
	//������Ʈ Ǯ��
public:
	static void Render();
	static void Update();

	static void Create(int pool);
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static shared_ptr<Bullet> Load(wstring imgfile);

private:
	static vector<shared_ptr<Bullet>> m;
	static int idx;

};

class Effect_Manager
{
	//������Ʈ Ǯ��
public:
	static void Create(int idx);
	static shared_ptr<class Animation> Load(wstring imgfile);

	static void Render();
	static void Update();
	static void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	static void Level_Up(D3DXVECTOR3 pos);
	/*  �����丵  */

private:
	
	static vector<Shader*> shader_vec;
	static int shader_idx; // �迭�� ����Ž�� ����

	static vector<ID3D11ShaderResourceView*> srv_vec;
	static vector<vector<ID3D11Buffer*>> buffer_vec;

	static vector<shared_ptr<Animation>> animations;

};

