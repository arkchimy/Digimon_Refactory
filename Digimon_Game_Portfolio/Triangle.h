#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	Triangle(wstring shaderFile);
	Triangle(wstring shaderFile, vec3 v0, vec3 v1, vec3 v2);
	~Triangle();

	void Update();
	void Render();

public:

	void CreateBuffer(wstring shaderFile);
	virtual void UpdateWorld() override; // 위치 스케일 변화 등
	vec3 ScreenCoordinate();

	virtual Hit CheckRayCollision(Ray& ray) override;
	bool IntersectRayTriangle(const vec3& orig, const vec3& dir,
		const vec3& v0, const vec3& v1,
		const vec3& v2, vec3& point, vec3& faceNormal,
		float& t, float& u, float& v);
public:
	vec3 v0, v1, v2;
private:
	
	
};

