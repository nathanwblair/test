#pragma once

#include "Utils.h"

class BoundingSphere
{
public:
	glm::vec3 center;
	float radius;

	BoundingSphere();

	void Fit(const vector<glm::vec3>& points);

	bool IsTouchingFrustumPlanes(const glm::mat4& transform, glm::vec3 positionOfBoundingSphere);
};

