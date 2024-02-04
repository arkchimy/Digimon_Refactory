#pragma once
#include "Actors/ICombat.h"
#define �л���Ÿ�� 1
#define ������Ÿ�� 2
#define ������Ÿ�� 3
#define ������Ÿ�� 4

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
	Attacker(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info,int level);
	virtual ~Attacker();
	void init_info(wstring imgfile, float width, float height, Sprites_Info info, int level);
	void init_info(wstring imgfile, vector<D3DXVECTOR4> uvs, Sprites_Info info, int level);

	virtual void FindLookAtTarget() = 0;
	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir) ;

	virtual void Update();
	virtual void Render();
	virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	virtual void UpdateWorld();
	

	virtual void Position(D3DXVECTOR3 val);
	D3DXVECTOR3 Position();
	virtual void Scale(D3DXVECTOR3 val);
	D3DXVECTOR3 Scale();
	virtual void Rotator(D3DXVECTOR3 val);
	D3DXVECTOR3 Rotator();

	shared_ptr<class Sprite> Get_Sprite();


	virtual bool IsDeathMode() override;
	virtual Buff_State Buff() { return buff_state; }

	void Set_IdleMode();
	void Set_WalkMode() { State = Walk; }
	void Set_StunMode();
	void Set_SkillMode() { State = Skill; }
	
	void Decal_Visible(bool val);
public:
	bool bBattle = false;
	wstring cut_SceanFile;
protected:
	UINT State;
	Buff_State buff_state = Buff_State::None;
	//Animation
	vector<vector<shared_ptr<class Sprite>>> sprites_vec;
	vector<unique_ptr<class Animation>> animations;
	unique_ptr<class Decal> decal;
	
	
	// =========== Sprite Info ============

	float Inter_Second = 1.5f;
	float Fire_time = 0.f;  // �ð���� �뵵
	float index = 0;  //�ε��� Delta�� ���ϸ� ����

	wstring Normal_Sprite_File;
	wstring Skill_Sprite_File;

	float Bullet_Speed;
	float Skill_Speed;
	float Skill_Percent;

	unsigned int Fire_idx : 4; // �߻��ϴ� Ÿ�̹�
	unsigned int Skill_idx : 4; // �߻��ϴ� Ÿ�̹�
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
};