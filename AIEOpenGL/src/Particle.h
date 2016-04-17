#pragma once
#include "Utils.h"

struct Particle {
	Particle() : lifetime(1), lifespan(0) {}

	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec4 color;
	
	float lifetime;
	float lifespan;

};
