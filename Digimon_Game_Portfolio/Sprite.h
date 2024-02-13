#pragma once

class Sprite 
{
public:
	Sprite(wstring imgFile, wstring shaderFile = Texture_Shader);
	Sprite(wstring imgFile, float End_X, float End_Y, wstring shaderFile = Texture_Shader);
	Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y, wstring shaderFile = Texture_Shader);
	
	Sprite(vector<ID3D11Buffer*>& ownerbuffer, wstring imgFile, wstring shaderFile = Texture_Shader);
	Sprite(vector<ID3D11Buffer*>& ownerbuffer, wstring imgFile, float start_X, float start_Y, float End_X, float End_Y, wstring shaderFile = Texture_Shader);

	virtual void CreateBuffer(wstring imgFile, wstring shaderFile) ;


	
	void Init_Sprite(wstring imgFile, float start_X, float start_Y, float End_X, float End_Y); // startX ,startY EndX, EndY
public:
	wstring img;

	typedef struct OBB_Desc
	{
		D3DXVECTOR3 position;

		D3DXVECTOR3 Obb_up;
		D3DXVECTOR3 Obb_rigth;
		float length_Up;
		float length_rigth;
	};

	static bool AABB(const D3DXVECTOR3 pos1, const D3DXVECTOR3 pos2);


	

private:
	

	D3DXMATRIX world;
	Vertex vertices[6];

	//Bound Shader
	Shader* BoundShader;
	ID3D11Buffer* BoundBuffer;
	struct BoundVertex
	{
		D3DXVECTOR3 postion;
	};
	virtual void CreateBoundBuffer(wstring shaderFile);

	ID3D11Buffer* vertexBuffer;
	class Shader* shader;
	ID3D11ShaderResourceView* srv;

};

class Sprite_Manager
{
	friend class Sprite;
public:
	static ID3D11ShaderResourceView* Load(wstring img);
	static vector<ID3D11Buffer*> LoadBuffer(wstring img);
	static vector<vector<ID3D11Buffer*>> LoadBufferVector(wstring img);
	
	static void StoreBuffer(wstring imgfile, vector<ID3D11Buffer*> buffer_vec); //총알같이 State가 없을떄
	static void StoreBufferVector(wstring imgfile, vector<vector<ID3D11Buffer*>> buffer_vec);
	

	static void ReMove(wstring img);
	
	struct Sprite_Desc
	{
	public:
		UINT RefCount = 0;
		ID3D11ShaderResourceView* srv = nullptr;
	};
	static map<wstring, Sprite_Desc> m;
	static map<wstring, vector<ID3D11Buffer*>> m_buffer;
	static map<wstring, vector<vector<ID3D11Buffer*>>> m_buffer_vec;
};