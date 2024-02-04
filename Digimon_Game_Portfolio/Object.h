#pragma once



using namespace glm;
class Object
{
public:
   
    vec3 amb = vec3(0.2f, 0.0f, 0.0f);  // Ambient
    vec3 dif = vec3(1.0f, 0.1f, 0.1f); // Diffuse
    vec3 spec = vec3(1.5f); // Specular
    float alpha = 50.0f;

    virtual void UpdateWorld() {};
    //virtual Hit CheckRayCollision(Ray& ray) = 0;

    virtual void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P);

public:
    void Position(float x, float y , float z);
    void Position(D3DXVECTOR3 vec);
    D3DXVECTOR3 Position() { return position; }

    void Scale(float x, float y,float z);
    virtual void Scale(D3DXVECTOR3 vec);
    D3DXVECTOR3 Scale() { return scale; }

    void Rotator(float x, float y, float z);
    virtual void Rotator(D3DXVECTOR3 vec);
    D3DXVECTOR3 Rotator() { return rotator; }

    void Color(float r, float g, float b);
    virtual void Color(D3DXCOLOR vec);
    D3DXCOLOR Color() { return color; }


    virtual void CreateBuffer(wstring shaderFile) {}; //override го╠Б
protected:
    D3DXVECTOR3 position = {0,0,0};
    D3DXVECTOR3 scale = { 1,1,1 };
    D3DXVECTOR3 rotator = { 0,0,0 };
    D3DXCOLOR color = D3DXCOLOR(0,0,0,1);
protected:
    ID3D11Buffer* vertexBuffer;
    class Shader* shader;
    ID3D11ShaderResourceView* srv;
};

