#pragma once
#include "Utils.h"

struct Particle
{
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 color;
	};

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;

	float size;
	float lifetime;
	float lifespan;
};
