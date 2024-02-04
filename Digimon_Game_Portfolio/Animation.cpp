#include "stdafx.h"
#include "Animation.h"
#include "Sprite.h"
#include "Actors/Attacker.h"
#include "Digimon.h"
#include "Actors/Enemy.h"


Animation::Animation(vector<shared_ptr<class Sprite>> data, PlayMode type)
	:sprites_vec(data),mode(type),playtime(0.f),index(0),play_rate(9.f)
{

}

Animation::Animation(Sprite_Info info, PlayMode type)
	: mode(type), playtime(0.f), index(0), play_rate(9.f)
{
	sprites_vec.resize(info.Sprite_cnt);

	float start_w = 0, start_h = 0;
	int unit_w = info.Width;
	int unit_h = info.Height;

	for (int i = 0; i < sprites_vec.size(); i++)
	{
		sprites_vec[i] = make_shared<Sprite>(info.ImgFile, start_w, start_h, start_w + unit_w, unit_h);
		start_w += unit_w;
	}

}

void Animation::Update()
{
	
	playtime += play_rate * Time::Delta();
	index = floor(playtime);
	Enemy* enemy = dynamic_cast<Enemy*>(Owner);
	switch (mode)
	{
	case PlayMode::Loop:
		if (index >= sprites_vec.size()) 
		{
			index = 0;
			playtime = 0.f;
		}
		break;
	case PlayMode::End:
		if (index >= sprites_vec.size())
		{
			if (Owner == nullptr)// Owner가 없는경우
			{
				index = sprites_vec.size() - 1; //마지막 상태 유지
				bvisible = false;
			}
			else // Owner가 있는경우
			{
				CheckTrue(Owner->Buff() == Buff_State::Stun);
				Enemy* enemy = dynamic_cast<Enemy*>(Owner);
				if (enemy == nullptr)
					Owner->Set_IdleMode();
				else
					Owner->Set_WalkMode();
			}
		}
		break;
	case PlayMode::End_Stop:
		
		if (index >= sprites_vec.size()) 
		{
			index = sprites_vec.size() - 1;

			if (enemy != nullptr)
			{
				if (enemy->IsDeathMode())
					enemy->Position({ 0, 1000, 0 });
			}
		}
		break;
	case PlayMode::Reverse:
		break;
	default:
		break;
	}
}

void Animation::Render()
{
	if (index >= sprites_vec.size())
		index = sprites_vec.size() - 1;
	sprites_vec[index]->Render();
}

void Animation::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (shared_ptr<class Sprite> sprite: sprites_vec)
	{
		sprite->ViewProjection(V, P);
	}
}

void Animation::UpdateWorld()
{
	if (index >= sprites_vec.size())
		index = sprites_vec.size() - 1;
	sprites_vec[index]->UpdateWorld();
}

void Animation::Position(D3DXVECTOR3 val)
{
	for (shared_ptr<class Sprite> sprite : sprites_vec)
	{
		sprite->Position(val);
	}
	UpdateWorld();
}

void Animation::Scale(D3DXVECTOR3 val)
{
	for (shared_ptr<class Sprite> sprite : sprites_vec)
	{
		sprite->Scale(val);
	}
	UpdateWorld();
}

D3DXVECTOR3 Animation::Position()
{
	if (index >= sprites_vec.size())
		index = sprites_vec.size() - 1;
	return sprites_vec[index]->Position();
}

D3DXVECTOR3 Animation::Scale()
{
	if (index >= sprites_vec.size())
		index = sprites_vec.size() - 1;
	return sprites_vec[index]->Scale();
}
