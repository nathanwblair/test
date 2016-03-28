#include "ParticleEmitter.h"



ParticleEmitter::ParticleEmitter()
	: Asset(),
	firstDead(0),
	maxParticles(0),
	position(0, 0, 0)
{
}


ParticleEmitter::~ParticleEmitter()
{
	renderData.DestroyBuffers(RenderData::Buffers::ALL);
}


void ParticleEmitter::Init(uint _maxParticles,
	uint _emitRate,
	float _minLifespan,
	float _maxLifespan,
	float _minVelocity,
	float _maxVelocity,
	float _startSize,
	float _endSize,
	const glm::vec4& _startColor,
	const glm::vec4& _endColor)
{	
	emitRate = 1.0f / _emitRate;
	emitTimer = 0;

	startColor = _startColor;
	endColor = _endColor;
	startSize = _startSize;
	endSize = _endSize;
	minVelocity = _minVelocity;
	maxVelocity = _maxVelocity;
	maxLifespan = _maxLifespan;
	minLifespan = _minLifespan;
	maxParticles = _maxParticles;

	particles.resize(maxParticles);
	firstDead = 0;

	vertices.resize(maxParticles * 4);

	vector<uint> indices;
	indices.resize(maxParticles * 6);

	for (uint i = 0; i < maxParticles; ++i)
	{
		auto offset = i * 6;
		auto resultOffset = i * 4;

		indices[offset + 0] = resultOffset + 0;
		indices[offset + 1] = resultOffset + 1;
		indices[offset + 2] = resultOffset + 2;

		indices[offset + 3] = resultOffset + 0;
		indices[offset + 4] = resultOffset + 2;
		indices[offset + 5] = resultOffset + 3;
	}

	renderData.GenerateBuffers(RenderData::Buffers::ALL);

	renderData.Bind();

	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 *
		sizeof(Particle::Vertex), vertices.data(),
		GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxParticles * 6 *
		sizeof(uint), indices.data(),
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(Particle::Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(Particle::Vertex), (void*)offsetof(Particle::Vertex, color));

	renderData.Unbind();
}


void ParticleEmitter::InitShaderProgram()
{
	shaderProgram.Load("data/shaders/particle.shader");
}


void ParticleEmitter::Emit()
{
	if (firstDead >= maxParticles)
		return;

	// resurrect the first dead particle
	Particle& particle = particles[firstDead++];

	// assign its starting position
	particle.position = position;

	// randomize its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) *
		(maxLifespan - minLifespan) + minLifespan;

	// set starting size and colour
	particle.color = startColor;
	particle.size = startSize;

	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) *
		(maxVelocity - minVelocity) + minVelocity;

	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;

	particle.velocity = glm::normalize(particle.velocity) * velocity;
}


void ParticleEmitter::Update(float deltaTime, const glm::mat4& cameraTransform)
{
	using glm::vec3;
	using glm::vec4;

	// Spawn particles
	emitTimer += deltaTime;
	while (emitTimer > emitRate)
	{
		Emit();
		emitTimer -= emitRate;
	}

	uint quad = 0;
	// update particles and turn live particles into billboard quads
	for (uint i = 0; i < firstDead; i++)
	{
		Particle* particle = &particles[i];
		particle->lifetime += deltaTime;
		if (particle->lifetime >= particle->lifespan) {
			// swap last alive with this one
			*particle = particles[firstDead - 1];
			firstDead--;
		}
		else
		{
			// move particle
			particle->position += particle->velocity * deltaTime;

			// size particle
			particle->size = glm::mix(startSize, endSize,
				particle->lifetime / particle->lifespan);

			// color particle
			particle->color = glm::mix(startColor, endColor,
				particle->lifetime / particle->lifespan);

			// make a quad the correct size and color
			float halfSize = particle->size * 0.5f;
			auto offset = quad * 4;

			vertices[offset + 0].position = vec4(halfSize, halfSize, 0, 1);
			vertices[offset + 0].color = particle->color;

			vertices[offset + 1].position = vec4(-halfSize, halfSize, 0, 1);
			vertices[offset + 1].color = particle->color;

			vertices[offset + 2].position = vec4(-halfSize, -halfSize, 0, 1);
			vertices[offset + 2].color = particle->color;

			vertices[offset + 3].position = vec4(halfSize, -halfSize, 0, 1);
			vertices[offset + 3].color = particle->color;

			// Create billboard transform
			vec3 zAxis = glm::normalize(vec3(cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);

			glm::mat4 billboard(vec4(xAxis, 0),
				vec4(yAxis, 0),
				vec4(zAxis, 0),
				vec4(0, 0, 0, 1));

			vertices[quad * 4 + 0].position = billboard *
				vertices[quad * 4 + 0].position +
				vec4(particle->position, 0);

			vertices[quad * 4 + 1].position = billboard *
				vertices[quad * 4 + 1].position +
				vec4(particle->position, 0);

			vertices[quad * 4 + 2].position = billboard *
				vertices[quad * 4 + 2].position +
				vec4(particle->position, 0);

			vertices[quad * 4 + 3].position = billboard *
				vertices[quad * 4 + 3].position +
				vec4(particle->position, 0);

			quad++;

		}
	}
}


void ParticleEmitter::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, renderData.GetVBO());
	

	glBufferSubData(GL_ARRAY_BUFFER, 0, firstDead * 4 * sizeof(Particle::Vertex), vertices.data());
	glBindVertexArray(renderData.GetVAO());

	glDrawElements(GL_TRIANGLES, firstDead * 6, GL_UNSIGNED_INT, 0);

	renderData.Unbind();
}
