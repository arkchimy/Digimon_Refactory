#include "stdafx.h"

#include "Light.h"

#include "Object.h"
#include "Hit.h"
#include "Ray.h"

using namespace glm;

Light Raytracer::light;

Raytracer::Raytracer(const int& width, const int& height)
	: width(width), height(height)
{
	light = Light{ {0.0f, 1.0f, 0.2f} }; // 화면 뒷쪽
}

Hit Raytracer::FindClosestCollision(Ray& ray)
{
	float closestD = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, vec3(0.f), vec3(0.f) };

	for (int i = 0; i < objects.size(); i++)
	{
		auto hit = objects[i]->CheckRayCollision(ray);

		if (hit.d >= 0.0f)
		{
			if (hit.d < closestD)
			{
				closestD = hit.d;
				closestHit = hit;
				closestHit.obj = objects[i];
			}
		}
	}

	return closestHit;
}

vec3 Raytracer::traceRay(Ray& ray)
{
	// Render first hit
	const auto hit = FindClosestCollision(ray);

	if (hit.d >= 0.0f)
	{
		// Diffuse
		const vec3 dirToLight = normalize(light.pos - hit.point);
		const float diff = std::max(dot(hit.normal, dirToLight), 0.0f);

		// Specular
		const vec3 reflectDir = 2.0f * dot(hit.normal, dirToLight) * hit.normal - dirToLight;
		const float specular = pow(std::max(dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

		Ray shadowRay;
		shadowRay.start = hit.point + vec3(1e-3, 1e-3, 1e-3);
		shadowRay.dir = normalize(light.pos - shadowRay.start);
		const auto shdowHit = FindClosestCollision(shadowRay);
		if (shdowHit.d >= 0)//아무도 맞지않았다면
		{
			return hit.obj->amb;
		}

		return hit.obj->amb + hit.obj->dif * diff + hit.obj->spec * specular;
	}

	return vec3(0.0f);
}

void Raytracer::Render(std::vector<vec4>& pixels)
{
	std::fill(pixels.begin(), pixels.end(), vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));
			Ray pixelRay{ pixelPosWorld, normalize(pixelPosWorld - eyePos) };
			pixels[i + width * j] = vec4(clamp(traceRay(pixelRay), 0.0f, 1.0f), 1.0f);
		}
	
}

void Raytracer::Add_Obejct(std::shared_ptr<Object> obj)
{
	objects.push_back(obj);
}

vec3 Raytracer::TransformScreenToWorld(vec2 posScreen)
{
	const float xScale = 2.0f / width;
	const float yScale = 2.0f / height;
	const float aspect = float(width) / height;

	// 3차원 공간으로 확장 (z좌표는 0.0)
	return vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
}