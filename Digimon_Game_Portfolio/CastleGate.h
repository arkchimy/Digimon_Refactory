#pragma once

class CastleGate 
{
public:
	CastleGate();
	~CastleGate();
	void Render();

	void UpdateWorld();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

	D3DXVECTOR3 Slot_Position();
	void ReturnSlot(D3DXVECTOR3 slotpos); //½½·Ô ¹ÝÈ¯
protected:
	vector<D3DXVECTOR3> slot_position;
	vector<bool> slot_use;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_rotator;

	unique_ptr<class Shader> shader;
	ComPtr<ID3D11ShaderResourceView> srv;
	vector<ComPtr<ID3D11Buffer>> buffer;

};

