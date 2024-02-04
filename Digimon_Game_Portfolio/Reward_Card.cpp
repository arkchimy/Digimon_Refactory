#include "stdafx.h"
#include "Reward_Card.h"
#include "Animation.h"
#include "Sprite.h"
#include "Stages/Stage.h"
#include "Digimon.h"

#define NormalScale D3DXVECTOR3{300,300,1}
#define OverScale D3DXVECTOR3{350,350,1}

#define CardPool 5
Reward_Card::Reward_Card(wstring Cardfile, const wstring ShaderFile)
	:digimon(nullptr)
{
	vector<shared_ptr<Sprite>> info;
	info.emplace_back(make_shared<Sprite>(Cardfile, ShaderFile));
	sprites = make_unique<Animation>(info, PlayMode::End_Stop);
	sprites->Scale(NormalScale);
	cardname = Cardfile;
	for (int i = 0; i < CardPool; i++)
	{
		if (Cardfile.compare(Card_Folder + L"테리어몬_카드.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"테리어몬.png", Terriermon_UV,
				Terriermon,
				성장기);
			
		}
		else if (Cardfile.compare(Card_Folder + L"레나몬_카드.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"레나몬.png", Renamon_UV,
				Renamon,
				성장기);
		}
		else if (Cardfile.compare(Card_Folder + L"가르고몬_카드.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"가르고몬.png", Galgomon_UV,
				Galgomon,
				성장기);
		}
		CheckNull(digimon); 
		q.push(digimon);
	}
	
}

Reward_Card::~Reward_Card()
{

}

void Reward_Card::Update()
{
	CheckFalse(sprites->Visible());
	D3DXVECTOR2 current_pos;
	current_pos.x = Mouse_Pos.x - Width / 2.f;
	current_pos.y = -(Mouse_Pos.y - Height / 2.f);
	//충돌 체크  AABB 구현해도될듯
	bool x_chk = current_pos.x <= sprites->Position().x + sprites->Scale().x / 2.f && current_pos.x >= sprites->Position().x - sprites->Scale().x / 2.f;
	bool y_chk = current_pos.y <= sprites->Position().y + sprites->Scale().y / 2.f && current_pos.y >= sprites->Position().y - sprites->Scale().y / 2.f;
	
	if (x_chk && y_chk) 
	{
		sprites->Scale(OverScale);
		bOver = true;
	}
	else 
	{
		sprites->Scale(NormalScale);
		bOver = false;
	}
}

void Reward_Card::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	sprites->ViewProjection(V, P);
	CheckTrue(q.empty());
	for (int i = 0; i < CardPool; i++)
	{
		auto front = q.front();
		q.pop();
		q.push(front);
		front->ViewProjection(V, P);
	}
}

void Reward_Card::Render()
{
	CheckFalse(sprites->Visible());
	sprites->Render();
}

void Reward_Card::Scale(D3DXVECTOR3 val)
{
	sprites->Scale(val);
}

void Reward_Card::Position(D3DXVECTOR3 val)
{
	sprites->Position(val);
}

void Reward_Card::Visible(bool val)
{
	sprites->Visible(val);
}

void Reward_Card::Use_Card()
{
	CheckTrue(q.empty());
	digimon = q.front();
	q.pop();
	q.push(digimon);
	digimon->Position({0,0,0});
	Owner->Add_Digimon(digimon);

	digimon = nullptr;
	
}

shared_ptr<class Digimon> Reward_Card::ReplaceDigimon()
{
	if (q.empty())
		return nullptr;
	digimon = q.front();
	q.pop();
	q.push(digimon);
	
	return digimon;
}

/*
* Card_Manager
*/
queue<shared_ptr<Reward_Card>>Card_Manager::q;


void Card_Manager::Create(vector<wstring> data)
{
	for (wstring file : data)
	{
		q.emplace(make_shared<Reward_Card>(file));
	}
}


shared_ptr<Reward_Card> Card_Manager::Load()
{
	shared_ptr<Reward_Card> front = nullptr;

	if (!q.empty()) 
	{
		front = q.front();
		q.pop();
		q.push(front); // 임시로 순환되게 만듦
	}
	return front;
}

shared_ptr<Digimon> Card_Manager::FindDigimon(wstring target)
{
	queue<shared_ptr<Reward_Card>> temp = q;
	shared_ptr<Reward_Card> front;
	while (!temp.empty())
	{
		front = temp.front();
		temp.pop();
		if (front->cardname.compare(target) == 0)
		{
			return front->ReplaceDigimon();
		}
	}
	return front->ReplaceDigimon();
}

void Card_Manager::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	queue<shared_ptr<Reward_Card>> temp = q;
	while (!temp.empty())
	{
		shared_ptr<Reward_Card> front = temp.front();
		temp.pop();
		front->ViewProjection(V, P);
	}
}

void Card_Manager::Use_Card()
{
	queue<shared_ptr<Reward_Card>> temp = q;
	shared_ptr<Reward_Card> target = nullptr;
	while (!temp.empty())
	{
		shared_ptr<Reward_Card> front = temp.front();
		temp.pop();
		if (front->MouseOver()) { // 마우스가 올려져있다면
			target = front;
			break;
		}
	}
	CheckNull(target);

	Card_Manager::Init_Visible();
	target->Use_Card();
}

void Card_Manager::Init_Visible()
{

	queue<shared_ptr<Reward_Card>> temp = q;
	while (!temp.empty())
	{
		shared_ptr<Reward_Card> front = temp.front();
		temp.pop();
		front->MouseOver(false);
	}

}

