#include "stdafx.h"
#include "Animation.h"
#include "Sprite.h"
#include "Actors/Attacker.h"
#include "Digimon.h"
#include "Actors/Enemy.h"


Animation::Animation(vector<shared_ptr<class Sprite>> data, PlayMode type)
	:sprites_vec(data),mode(type),playtime(0.f),index(0),play_rate(Sprite_Speed)
{

}

Animation::Animation(Sprite_Info info, PlayMode type)
	: mode(type), playtime(0.f), index(0), play_rate(Sprite_Speed)
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


Animation::Animation(Shader* shader, ComPtr<ID3D11ShaderResourceView> srv_vec, vector<ComPtr<ID3D11Buffer>> buffer_vec, PlayMode mode)
	: m_Shader(shader), m_srv(srv_vec), m_buffer_vec(buffer_vec), mode(mode), play_rate(Sprite_Speed)
{

}

void Animation::UpdateSrvAndBuffer(ComPtr<ID3D11ShaderResourceView> srv, vector<ComPtr<ID3D11Buffer>> buffer_vec)
{
	m_srv = srv;
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
		if (index >= m_buffer_vec.size())
		{
			index = 0;
			playtime = 0.f;
		}
		break;
	case PlayMode::End:
		if (index >= m_buffer_vec.size())
		{
			if (Owner == nullptr)// Owner가 없는경우
			{
				index = m_buffer_vec.size() - 1; //마지막 상태 유지
				bvisible = false;
			}
			else // Owner가 있는경우
			{
				CheckTrue(Owner->Buff() == Buff_State::Stun);
				Enemy* enemy = dynamic_cast<Enemy*>(Owner);
				if (enemy == nullptr)
					Owner->Set_Mode(IDLE);
				else
					Owner->Set_Mode(Walk);
			}
		}
		break;
	case PlayMode::End_Stop:
		
		if (index >= m_buffer_vec.size())
		{
			index = m_buffer_vec.size() - 1;

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

	if (index >= m_buffer_vec.size())
		index = m_buffer_vec.size() - 1;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_Shader->AsShaderResource("Map")->SetResource(m_srv.Get());

	DeviceContext->IASetVertexBuffers(0, 1, m_buffer_vec[index].GetAddressOf(), &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader->Draw(0, 0, 6);
	
}

void Animation::ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P)
{
	//m_Shader->a
}

void Animation::UpdateWorld()
{
	D3DXMATRIX W, S, R, T;

	D3DXMatrixScaling(&S, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, m_rotator.y, m_rotator.x, m_rotator.z);
	D3DXMatrixTranslation(&T, m_position.x, m_position.y, m_position.z);
	W = S * R * T;
	m_Shader->AsMatrix("World")->SetMatrix(W);
}

void Animation::Position(D3DXVECTOR3 val)
{
	CheckNull(m_Shader);

	m_position = val;
	UpdateWorld();
}

void Animation::Scale(D3DXVECTOR3 val)
{
	CheckNull(m_Shader);

	m_scale = val;
	UpdateWorld();
}

D3DXVECTOR3 Animation::Position()
{
	return m_position;
}

D3DXVECTOR3 Animation::Scale()
{
	return m_scale;
}