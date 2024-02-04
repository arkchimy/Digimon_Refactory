#include "stdafx.h"
#include "Player.h"

#include "Sprite.h"
#include "Digimon.h"


extern Sprites_Info Guilmon;
Player::Player( wstring shaderFile)
{
	My_Digimon.reserve(3);
	My_Digimon.push_back(make_shared<Digimon>(Digimon_Folder + L"길몬.png", 40, 40,
		Guilmon,
		성장기));
	//My_Digimon.push_back(make_unique<Digimon>(Digimon_Folder + L"레나몬.png", Renamon_UV,
	//	Renamon,
	//	성장기));
	//My_Digimon.push_back(make_unique<Digimon>(Digimon_Folder + L"테리어몬.png", Terriermon_UV,
	//	Terriermon,
	//	성장기));
	

	cards.push_back(Card_Folder + L"테리어몬_카드.png");
	cards.push_back(Card_Folder + L"레나몬_카드.png");
	cards.push_back(Card_Folder + L"가르고몬_카드.png");
}

Player::~Player()
{

}

void Player::Update()
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		if (i == 1)
			i = i;
		My_Digimon[i]->Update();
	}
}

void Player::Render()
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->Render();
	}
}

void Player::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->ViewProjection(V, P);
	}
}

void Player::UpdateWorld()
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->UpdateWorld();
	}
}

void Player::Ready()
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->Set_IdleMode();
	}
}

void Player::Battle(bool val)
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->Battle(val);
	}
}

void Player::Stage_Clear()
{
	for (int i = 0; i < My_Digimon.size(); i++)
	{
		My_Digimon[i]->Stage_Clear();
	}
}

void Player::Slot_Set(vector<D3DXVECTOR3> slot_pos)
{
	for (int i = 0; i < slot_pos.size(); i++) 
	{
		if (My_Digimon.size() <= i)
			break;
		My_Digimon[i]->Position(slot_pos[i]);
	}
}

void Player::Use_Skill(int idx,D3DXVECTOR2 MousePos)
{
	idx -=1 ;
	CheckTrue(My_Digimon.size() <= idx);
	My_Digimon[idx]->Skill_Target_Pos = MousePos;
	My_Digimon[idx]->Set_SkillMode();
	My_Digimon[idx]->Animation_Start(Skill);
}

void Player::Decal_Visible(int idx,bool val)
{
	idx -= 1;
	CheckTrue(My_Digimon.size() <= idx);
	My_Digimon[idx]->Decal_Visible(val);
}

void Player::ClickEvent()
{
	D3DXVECTOR2 hit_point;
	hit_point.x = Mouse_Pos.x - Width / 2.f;
	hit_point.y = -(Mouse_Pos.y - Height / 2.f);

	for (int i = 0; i < My_Digimon.size(); i++)
	{
		if (My_Digimon[i]->ClickEvent(hit_point))
		{
			drag_digimon = My_Digimon[i];
			return;
		}
			
	}
}

void Player::ClickUp_Event()
{
	CheckNull(drag_digimon);
	D3DXVECTOR2 hit_point;
	hit_point.x = Mouse_Pos.x - Width / 2.f;
	hit_point.y = -(Mouse_Pos.y - Height / 2.f);

	for (int i = 0; i < My_Digimon.size(); i++)
	{
		if (My_Digimon[i]->ClickEvent(hit_point, drag_digimon))
		{
			drag_digimon->Drag(false);
			auto iter = find(My_Digimon.begin(), My_Digimon.end(), drag_digimon);
			My_Digimon.erase(iter);
			shared_ptr<Digimon> chk = My_Digimon[i]->Evolution(); //진화 확인
			if (chk != nullptr)
			{
				D3DXVECTOR3 pos = My_Digimon[i]->Position();
				chk->Position(pos);
				My_Digimon[i] = chk;
				My_Digimon[i]->Set_IdleMode();
				My_Digimon[i]->Battle(배틀시작);
			}
			break;
		}

	}
	drag_digimon->Drag(false);
}

void Player::Add_Digimon(shared_ptr<Digimon> input)
{
	My_Digimon.push_back(input);
}

