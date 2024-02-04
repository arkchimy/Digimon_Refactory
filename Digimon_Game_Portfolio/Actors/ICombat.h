#pragma once
#define interface

interface class ICombat
{
public:
	virtual ~ICombat() {};
	virtual void Take_Damage(class Bullet* causer, D3DXVECTOR3 dir) = 0; // ���� �������� ����� �߿��Ѱ�?
	
	virtual void Fire() = 0;
	virtual bool IsDeathMode() = 0;
	//virtual void Skill() = 0;
};