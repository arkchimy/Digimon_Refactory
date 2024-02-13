#pragma once
#include "Actors/ICombat.h"
#include "Animation.h"
#include "Sprite_info.h"

#define 분산형타입 1
#define 연사형타입 2
#define 관통형타입 3
#define 레이저타입 4

enum Buff_State
{
	None = 0,
	Power_Up = 1 << 1,
	Speed_Up = 1 << 2,
	Slow = 1 << 3,
	Stun = 1 << 4,
	Dead = 1 << Death
};

class Attacker : public ICombat
{
public:

	Attacker(wstring imgfile, float width, float height, Sprites_Info info, int level);
	Attacker(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);
	virtual ~Attacker();

	void Init_info(const Sprites_Info& info);
	void CreateShaderAndBuffer(wstring imgfile, float width, float height, Sprites_Info info, int level);
	void CreateShaderAndBuffer(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);

	void CreateAnimation(); // Animation객체 
	void UpdateSrvAndBuffer(wstring imgfile);


	virtual void FindLookAtTarget() = 0;
	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir) = 0;

	virtual void Update();
	virtual void Render();
	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	virtual void UpdateWorld();


	/* Setter */
	virtual void Position(D3DXVECTOR3 val);
	virtual void Scale(D3DXVECTOR3 val);
	virtual void Rotator(D3DXVECTOR3 val); // 라이안 변환후 갱신
	/* Getter */
	FORCEINLINE D3DXVECTOR3 Position() { return position; }
	FORCEINLINE D3DXVECTOR3 Scale() { return scale; }
	FORCEINLINE D3DXVECTOR3 Rotator() { return rotator; }

	FORCEINLINE void Set_Mode(UINT mode) { State = mode; animations[State]->Start(); } // 자동으로 애니메이션 호출
	FORCEINLINE void Set_Mode(Buff_State val) { buff_state = val; Stun_Time = 0.2f;  }

	virtual bool IsDeathMode() override;

	virtual Buff_State Buff() { return buff_state; }
	


public:
	bool bBattle = false;
	wstring cut_SceanFile;
protected:

	UINT State;
	Buff_State buff_state = Buff_State::None;

	// =========== Sprite Info ============

	float Inter_Second = 1.5f;
	float Fire_time = 0.f;  // 시간재는 용도
	float index = 0;  //인덱스 Delta에 더하며 측정

	wstring Normal_Sprite_File;
	wstring Skill_Sprite_File;

	float Bullet_Speed;
	float Skill_Speed;
	float Skill_Percent;

	unsigned int Fire_idx : 4; // 발사하는 타이밍
	unsigned int Skill_idx : 4; // 발사하는 타이밍
	unsigned int Bullet_Type : 3;
	unsigned int Skill_Type : 3;



	int Normal_Bullet_Cnt = 1;
	int Skill_Bullet_Cnt = 1;
	//=================================
protected:
	UINT team_id;
	float hp = 3.f;
	float knock_back = 1.f;
	float Stun_Time = 0.f;
	D3DXVECTOR3 Move_dir;

	Vertex vertices[6];

	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* m_srv;
	vector<vector<ID3D11Buffer*>> buffer_vec;


	//
	float indexing = 0.f;
	int idx = indexing;


	D3DXMATRIX world;

	D3DXVECTOR3 scale = { 100,100,1 };
	D3DXVECTOR3 rotator = { 0,0,0 };
	D3DXVECTOR3 position = { 275.0,-185,0 };
	//=============== Animation 살리기 ===============
	protected:
		vector<unique_ptr<class Animation>> animations;
		


	
};
