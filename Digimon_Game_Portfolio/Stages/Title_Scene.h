#pragma once
#include "Stages//Stage.h"

class Title_Scene : public Stage
{
public:
	Title_Scene();
	~Title_Scene();
	void Update()override ;
	virtual void Init_Stage(shared_ptr<class Player> player);

	vector<D3DXVECTOR3> MainMap_Speed
	{
		{-13.f,0.f,0.f},
		{-70.f,0.f,0.f},
		{-0.029,0,0},
		{-0.079,0,0},
		{-0.010,0,0},
	};
	vector<D3DXVECTOR3> MainMap_GSpeed
	{
		{-115.f,0.f,0.f},
		{-1,0,0},
		{-1,0,0},
		{-0.029,0,0},
		{-0.079,0,0},
		{-0.010,0,0},
	};

};