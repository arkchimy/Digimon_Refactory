#pragma once

class Raytracer
{
	static class Light light;
	
	vector<shared_ptr<class Object>> objects;
public:

	Raytracer(const int& width, const int& height);
	class Hit FindClosestCollision(class Ray& ray);
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	glm::vec3 traceRay(class Ray& ray);
	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen);

	void Render(std::vector<glm::vec4>& pixels);

	void Add_Obejct(std::shared_ptr<class Object> obj);
	
public:
	float width, height;
};

