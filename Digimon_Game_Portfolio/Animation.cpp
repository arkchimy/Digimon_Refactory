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

Animation::Animation(Shader* shader, vector<ID3D11ShaderResourceView*> srv_vec, vector<ID3D11Buffer*> buffer_vec, PlayMode mode)
	:m_Shader(shader),m_srv_vec(srv_vec), m_buffer_vec(buffer_vec),mode(mode), play_rate(9.f)
{
	
}

void Animation::UpdateSrvAndBuffer(vector<ID3D11ShaderResourceView*> srv_vec, vector<ID3D11Buffer*> buffer_vec)
{
	m_srv_vec = srv_vec;
	m_buffer_vec = buffer_vec;
}

void Animation::Update()
{
	CheckFalse(bvisible);

	playtime += play_rate * ImGui::GetIO().DeltaTime;
	index = floor(playtime);
	Enemy* enemy = dynamic_cast<Enemy*>(Owner);
	switch (mode)
	{
	case PlayMode::Loop:
		if (index >= m_srv_vec.size())
		{
			index = 0;
			playtime = 0.f;
		}
		break;
	case PlayMode::End:
		if (index >= m_srv_vec.size())
		{
			if (Owner == nullptr)// Owner가 없는경우
			{
				index = m_srv_vec.size() - 1; //마지막 상태 유지
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
		
		if (index >= m_srv_vec.size())
		{
			index = m_srv_vec.size() - 1;

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
	CheckFalse(bvisible);

	if (index >= m_srv_vec.size())
		index = m_srv_vec.size() - 1;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_Shader->AsShaderResource("Map")->SetResource(m_srv_vec[index]);

	DeviceContext->IASetVertexBuffers(0, 1, &m_buffer_vec[index], &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader->Draw(0, 0, 6);
	
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
	CheckNull(m_Shader);
	D3DXMATRIX W, S, T;

	D3DXMatrixScaling(&S, 150, 150, 1);
	D3DXMatrixTranslation(&T, val.x, val.y, val.z);
	W = S * T;
	m_Shader->AsMatrix("World")->SetMatrix(W);
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