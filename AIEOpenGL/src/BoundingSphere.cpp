#include "BoundingSphere.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

BoundingSphere::BoundingSphere() : center(0),
                                   radius(0) 
{

}

void BoundingSphere::Fit(const vector<glm::vec3>& points) 
{

	vec3 min(1e37f);
	vec3 max(-1e37f);

	for (auto& p : points)
	{
		if (p.x < min.x)
			min.x = p.x;
		if (p.y < min.y)
			min.y = p.y;
		if (p.z < min.z)
			min.z = p.z;
		if (p.x > max.x)
			max.x = p.x;
		if (p.y > max.y)
			max.y = p.y;
		if (p.z > max.z)
			max.z = p.z;
	}

	center = (min + max) * 0.5f;
	radius = glm::distance(min, center);
}

vector<glm::vec4> GetFrustumPlanes(const glm::mat4& transform) 
{
	auto planes = vector<glm::vec4>(6);

	auto a = glm::vec4(transform[0][3],
	                   transform[1][3],
	                   transform[2][3],
	                   transform[3][3]);

	vector<glm::vec4> parts(3);

	for (uint i = 0; i < parts.size(); i++)
	{
		parts[i] = glm::vec4(transform[0][i],
		                     transform[1][i],
		                     transform[2][i],
		                     transform[3][i]);
	}

	for (uint i = 0; i < 6; i++)
	{
		if (i % 2 == 0)
			planes[i] = a - parts[i / 2];
		else
			planes[i] = a + parts[i / 2];
	}

	for (uint i = 0; i < 6; i++)
	{
		planes[i] /= glm::length(vec3(planes[i]));
	}

	return planes;
}

bool BoundingSphere::IsTouchingFrustumPlanes(const glm::mat4& transform, glm::vec3 positionOfBoundingSphere) 
{
	auto planes = GetFrustumPlanes(transform);

	for (uint i = 0; i < 6; i++)
	{
		float d = glm::dot(vec3(planes[i]), center + (positionOfBoundingSphere)) + planes[i].w;

		if (d < -radius)
		{
			return false;
		}
	}

	return true;
}