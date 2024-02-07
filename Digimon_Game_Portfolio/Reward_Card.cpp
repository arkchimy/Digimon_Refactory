#include "stdafx.h"
#include "Reward_Card.h"
#include "Animation.h"
#include "Sprite.h"
#include "Stages/Stage.h"
#include "Digimon.h"

#include <random>
#define NormalScale D3DXVECTOR3{300,300,1}
#define OverScale D3DXVECTOR3{350,350,1}

#define CardPool 5
Reward_Card::Reward_Card(wstring Cardfile, const wstring ShaderFile)
	:digimon(nullptr)
{
	vector<shared_ptr<Sprite>> info;
	info.emplace_back(make_shared<Sprite>(Cardfile, ShaderFile));
	sprites = make_unique<Animation>(info, PlayMode::End_Stop);


	/* 리팩토링 */
	

	///
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

Reward_Card::Reward_Card()
{

}

Reward_Card::~Reward_Card()
{

}

void Reward_Card::Update()
{
	CheckNull(m_animation);

	// 마우스 current 위치
	D3DXVECTOR2 current_pos;
	current_pos.x = Mouse_Pos.x - Width / 2.f;
	current_pos.y = -(Mouse_Pos.y - Height / 2.f);
	//충돌 체크  AABB 구현해도될듯

	bool x_chk = current_pos.x <= m_position.x + m_scale.x / 2.f && current_pos.x >= m_position.x - m_scale.x / 2.f;
	bool y_chk = current_pos.y <= m_position.y + m_scale.y / 2.f && current_pos.y >= m_position.y - m_scale.y / 2.f;
	
	if (x_chk && y_chk) 
	{
		m_scale = OverScale;
		m_animation->Scale(m_scale);
		bOver = true;
	}
	else 
	{
		m_scale = NormalScale;
		m_animation->Scale(NormalScale);
		bOver = false;
	}
}

void Reward_Card::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	/*sprites->ViewProjection(V, P);
	CheckTrue(q.empty());
	for (int i = 0; i < CardPool; i++)
	{
		auto front = q.front();
		q.pop();
		q.push(front);
		front->ViewProjection(V, P);
	}*/
}

void Reward_Card::Render()
{
	CheckNull(m_animation);
	m_animation->Render();

}

void Reward_Card::Scale(D3DXVECTOR3 val)
{
	CheckNull(m_animation);

	m_scale = val;
	m_animation->Scale(val);
}

void Reward_Card::Position(D3DXVECTOR3 val)
{
	CheckNull(m_animation);

	m_position = val;
	m_animation->Position(val);

}

void Reward_Card::Visible(bool val)
{
	CheckNull(m_animation);
	m_animation->Visible(val);

}

void Reward_Card::Use_Card()
{
	
	CheckNull(digimon);
	digimon->Position({0,0,0});
	digimon = nullptr;
	
}

shared_ptr<class Digimon> Reward_Card::ReplaceDigimon()
{

	cout << "Reward_Card::ReplaceDigimon 호출" << endl;
	if (q.empty())
		return nullptr;
	digimon = q.front();
	q.pop();
	q.push(digimon);
	
	return digimon;
}

void Reward_Card::Load()
{
	int DigimonID;
	m_animation = Card_Manager::Load(DigimonID);
	
	if (DigimonID == 0)
	{
		digimon = make_shared<Digimon>(Digimon_Folder + L"테리어몬.png", Terriermon_UV,
			Terriermon,
			성장기);

	}
	else if (DigimonID == 1)
	{
		digimon = make_shared<Digimon>(Digimon_Folder + L"레나몬.png", Renamon_UV,
			Renamon,
			성장기);
	}
	else if (DigimonID == 2)
	{
		digimon = make_shared<Digimon>(Digimon_Folder + L"가르고몬.png", Galgomon_UV,
			Galgomon,
			성장기);
	}

	m_animation->Start();
}

/*
* Card_Manager
*/
queue<shared_ptr<Reward_Card>>Card_Manager::q;

//  리팩토링
vector<Shader*> Card_Manager::shader_vec;
vector<vector<ID3D11ShaderResourceView*>> Card_Manager::srv_vec;
vector<vector<ID3D11Buffer*>> Card_Manager::buffer_vec;

vector<shared_ptr<Animation>> Card_Manager::animations;


int Card_Manager::shader_idx = 0;

void Card_Manager::Create(vector<wstring> data)
{

	// shader와 animation은 딱 세개만 생성하였음.
	// 상황에 따라 srv 와 buffer 를 swap 하는 방식으로 구현.

	int cardTypeCnt = data.size();

	srv_vec.resize(cardTypeCnt);
	buffer_vec.resize(cardTypeCnt);

	for (int k = 0; k < data.size(); k++)
	{
		make_shared<Sprite>(srv_vec[k], buffer_vec[k], data[k], Card_Shader);
	}

	shader_vec.reserve(CardPoolSize);
	animations.reserve(CardPoolSize);
	for (int i = 0; i < CardPoolSize; i++)
	{
		shader_vec.emplace_back(new Shader(Card_Shader));
		animations.emplace_back(make_shared<Animation>(shader_vec[i],srv_vec[i],buffer_vec[i]));
	}
	
	
}

shared_ptr<class Animation> Card_Manager::Load(int& cardID)
{

	// 랜덤 주의!  rand() % 3 라고 작성했다가 계속 2만 나오는 현상나와서 바꿈
	std::random_device rd;
	std::mt19937 gen(rd());

	// 1부터 6까지의 균일 분포에서 정수를 생성합니다.
	std::uniform_int_distribution<> dis(0, 2);

	cardID = dis(gen);

	shader_idx++;
	shader_idx %= shader_vec.size();
	animations[shader_idx]->UpdateSrvAndBuffer(srv_vec[cardID], buffer_vec[cardID]);
	
	return animations[shader_idx];
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
	for (auto shader : shader_vec) 
	{
		shader->AsMatrix("View")->SetMatrix(V);
		shader->AsMatrix("Projection")->SetMatrix(P);
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
