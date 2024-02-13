#pragma once
class Bottom_UI
{

public:
	Bottom_UI();
	~Bottom_UI();

	void Update();
	void Render();
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);
	

	void UpdateWorld();

	vector<D3DXVECTOR3> m_position;
	vector<D3DXVECTOR3> m_scale;
	vector<D3DXVECTOR3> m_rotator;

	vector<class Shader*> m_shader;
	vector<vector<ComPtr<ID3D11Buffer>>> m_buffer;
	vector<ComPtr<ID3D11ShaderResourceView>> m_srv;


	static void GameClear(bool val) { bvictory = val; }
private:
	static bool bvictory;
};