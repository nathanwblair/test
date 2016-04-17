#include "ParticleEmitter.h"
#include <glm/gtc/type_ptr.hpp>


ParticleEmitter::ParticleEmitter()
	: Asset(),
	firstDead(0),
	maxParticles(0),
	position(0, 0, 0)
{
}


ParticleEmitter::~ParticleEmitter()
{
	renderData[0].DestroyBuffers(RenderData::Buffers::ALL);
	renderData[1].DestroyBuffers(RenderData::Buffers::ALL);

	shaderProgram[0].GLDelete();
	shaderProgram[1].GLDelete();
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

	SetupBuffer(renderData[0], maxParticles, vertices, indices);
	SetupBuffer(renderData[1], maxParticles, vertices, indices);

	currentBuffer = &renderData[0];
	otherBuffer = &renderData[1];
}


void ParticleEmitter::SetupBuffer(RenderData& renderer,
								int maxParticles, 
								const vector<Particle>& vertices, 
								const vector<uint>& indices)
{
	renderer.GenerateBuffers(RenderData::Buffers::ALL);

	renderer.Bind();

	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 *
		sizeof(Particle), vertices.data(),
		GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxParticles * 6 *
		sizeof(uint), indices.data(),
		GL_STATIC_DRAW);

	ClearVAOMetadata();
	subscribe(Particle, position);
	subscribe(Particle, color);

	renderer.Unbind();
}


void ParticleEmitter::InitShaderProgram()
{
	shaderProgram[0].Load("data/shaders/particle.shader");
	shaderProgram[1].Load("data/shaders/particleUpdate.shader", [](int id) {

		// specify the data that we will stream back
		const char* varyings[] = {
			"position",
			"velocity",
			"lifetime", 
			"lifespan" };

		glTransformFeedbackVaryings(id, 4, varyings,
			GL_INTERLEAVED_ATTRIBS);
	});

	shaderProgram[DRAW_SHADER].Use();

	shaderProgram[DRAW_SHADER].SetUniform("sizeStart", startSize);
	shaderProgram[DRAW_SHADER].SetUniform("sizeEnd", endSize);


	shaderProgram[DRAW_SHADER].SetUniform("colorStart", startColor);
	shaderProgram[DRAW_SHADER].SetUniform("colorEnd", endColor);

	shaderProgram[UPDATE_SHADER].Use();

	shaderProgram[UPDATE_SHADER].SetUniform("sizeStart", startSize);
	shaderProgram[UPDATE_SHADER].SetUniform("sizeEnd", endSize);


	shaderProgram[UPDATE_SHADER].SetUniform("lifeMin", minLifespan);
	shaderProgram[UPDATE_SHADER].SetUniform("lifeMax", maxLifespan);
}


//void ParticleEmitter::Emit()
//{
//	if (firstDead >= maxParticles)
//		return;
//
//	// resurrect the first dead particle
//	Particle& particle = particles[firstDead++];
//
//	// assign its starting position
//	particle.position = position;
//
//	// randomize its lifespan
//	particle.lifetime = 0;
//	particle.lifespan = (rand() / (float)RAND_MAX) *
//		(maxLifespan - minLifespan) + minLifespan;
//
//	// set starting size and colour
//	particle.color = startColor;
//	particle.size = startSize;
//
//	// randomise velocity direction and strength
//	float velocity = (rand() / (float)RAND_MAX) *
//		(maxVelocity - minVelocity) + minVelocity;
//
//	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
//	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
//	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
//
//	particle.velocity = glm::normalize(particle.velocity) * velocity;
//}


//void ParticleEmitter::Update(float deltaTime, const glm::mat4& cameraTransform)
//{
//	using glm::vec3;
//	using glm::vec4;
//
//	// Spawn particles
//	emitTimer += deltaTime;
//	while (emitTimer > emitRate)
//	{
//		Emit();
//		emitTimer -= emitRate;
//	}
//
//	uint quad = 0;
//	// update particles and turn live particles into billboard quads
//	for (uint i = 0; i < firstDead; i++)
//	{
//		Particle* particle = &particles[i];
//		particle->lifetime += deltaTime;
//		if (particle->lifetime >= particle->lifespan) {
//			// swap last alive with this one
//			*particle = particles[firstDead - 1];
//			firstDead--;
//		}
//		else
//		{
//			// move particle
//			particle->position += particle->velocity * deltaTime;
//
//			// size particle
//			particle->size = glm::mix(startSize, endSize,
//				particle->lifetime / particle->lifespan);
//
//			// color particle
//			particle->color = glm::mix(startColor, endColor,
//				particle->lifetime / particle->lifespan);
//
//			// make a quad the correct size and color
//			float halfSize = particle->size * 0.5f;
//			auto offset = quad * 4;
//
//			vertices[offset + 0].position = vec4(halfSize, halfSize, 0, 1);
//			vertices[offset + 0].color = particle->color;
//
//			vertices[offset + 1].position = vec4(-halfSize, halfSize, 0, 1);
//			vertices[offset + 1].color = particle->color;
//
//			vertices[offset + 2].position = vec4(-halfSize, -halfSize, 0, 1);
//			vertices[offset + 2].color = particle->color;
//
//			vertices[offset + 3].position = vec4(halfSize, -halfSize, 0, 1);
//			vertices[offset + 3].color = particle->color;
//
//			// Create billboard transform
//			vec3 zAxis = glm::normalize(vec3(cameraTransform[3]) - particle->position);
//			vec3 xAxis = glm::cross(vec3(cameraTransform[1]), zAxis);
//			vec3 yAxis = glm::cross(zAxis, xAxis);
//
//			glm::mat4 billboard(vec4(xAxis, 0),
//				vec4(yAxis, 0),
//				vec4(zAxis, 0),
//				vec4(0, 0, 0, 1));
//
//			vertices[quad * 4 + 0].position = billboard *
//				vertices[quad * 4 + 0].position +
//				vec4(particle->position, 0);
//
//			vertices[quad * 4 + 1].position = billboard *
//				vertices[quad * 4 + 1].position +
//				vec4(particle->position, 0);
//
//			vertices[quad * 4 + 2].position = billboard *
//				vertices[quad * 4 + 2].position +
//				vec4(particle->position, 0);
//
//			vertices[quad * 4 + 3].position = billboard *
//				vertices[quad * 4 + 3].position +
//				vec4(particle->position, 0);
//
//			quad++;
//
//		}
//	}
//}


void ParticleEmitter::Draw(float time, 
	const glm::mat4& cameraTransform, 
	const glm::mat4& projectionView)
{
	shaderProgram[UPDATE_SHADER].Use();

	shaderProgram[UPDATE_SHADER].SetUniform("time", time);

	float deltaTime = time - lastDrawTime; 
	lastDrawTime = time;
	shaderProgram[UPDATE_SHADER].SetUniform("deltaTime", deltaTime);

	shaderProgram[UPDATE_SHADER].SetUniform("emitterPosition", position);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(currentBuffer->GetVAO());

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, otherBuffer->GetVBO());
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, currentBuffer->GetVBO());
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	shaderProgram[DRAW_SHADER].Use();
	shaderProgram[DRAW_SHADER].SetUniform("projectionView", projectionView);
	shaderProgram[DRAW_SHADER].SetUniform("cameraTransform", cameraTransform);

	glBindVertexArray(currentBuffer->GetVAO());
	glDrawArrays(GL_POINTS, 0, maxParticles);

	auto tempBuffer = currentBuffer;
	currentBuffer = otherBuffer;
	otherBuffer = currentBuffer;
}
