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


	/* �����丵 */
	

	///
	sprites->Scale(NormalScale);
	cardname = Cardfile;
	for (int i = 0; i < CardPool; i++)
	{
		if (Cardfile.compare(Card_Folder + L"�׸����_ī��.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"�׸����.png", Terriermon_UV,
				Terriermon,
				�����);
			
		}
		else if (Cardfile.compare(Card_Folder + L"������_ī��.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"������.png", Renamon_UV,
				Renamon,
				�����);
		}
		else if (Cardfile.compare(Card_Folder + L"�������_ī��.png") == 0)
		{
			digimon = make_shared<Digimon>(Digimon_Folder + L"�������.png", Galgomon_UV,
				Galgomon,
				�����);
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

	// ���콺 current ��ġ
	D3DXVECTOR2 current_pos;
	current_pos.x = Mouse_Pos.x - Width / 2.f;
	current_pos.y = -(Mouse_Pos.y - Height / 2.f);
	//�浹 üũ  AABB �����ص��ɵ�

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

	cout << "Reward_Card::ReplaceDigimon ȣ��" << endl;
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
		digimon = make_shared<Digimon>(Digimon_Folder + L"�׸����.png", Terriermon_UV,
			Terriermon,
			�����);

	}
	else if (DigimonID == 1)
	{
		digimon = make_shared<Digimon>(Digimon_Folder + L"������.png", Renamon_UV,
			Renamon,
			�����);
	}
	else if (DigimonID == 2)
	{
		digimon = make_shared<Digimon>(Digimon_Folder + L"�������.png", Galgomon_UV,
			Galgomon,
			�����);
	}

	m_animation->Start();
}

/*
* Card_Manager
*/
queue<shared_ptr<Reward_Card>>Card_Manager::q;

//  �����丵
vector<Shader*> Card_Manager::shader_vec;
vector<vector<ID3D11ShaderResourceView*>> Card_Manager::srv_vec;
vector<vector<ID3D11Buffer*>> Card_Manager::buffer_vec;

vector<shared_ptr<Animation>> Card_Manager::animations;


int Card_Manager::shader_idx = 0;

void Card_Manager::Create(vector<wstring> data)
{

	// shader�� animation�� �� ������ �����Ͽ���.
	// ��Ȳ�� ���� srv �� buffer �� swap �ϴ� ������� ����.

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

	// ���� ����!  rand() % 3 ��� �ۼ��ߴٰ� ��� 2�� ������ ���󳪿ͼ� �ٲ�
	std::random_device rd;
	std::mt19937 gen(rd());

	// 1���� 6������ ���� �������� ������ �����մϴ�.
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
		if (front->MouseOver()) { // ���콺�� �÷����ִٸ�
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
