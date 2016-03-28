#pragma once
#include "Asset.h"

#include "Particle.h"
#include "RenderData.h"
#include "ShaderProgram.h"

class ParticleEmitter :
	public Asset
{
public:
	ParticleEmitter();
	~ParticleEmitter();

public:

	vector<Particle> particles;
	uint firstDead;
	uint maxParticles;

	//	GLRenderer renderer;

	glm::vec3 position;

	float emitTimer;
	float emitRate;

	float minLifespan;
	float maxLifespan;

	float minVelocity;
	float maxVelocity;

	float startSize;
	float endSize;

	glm::vec4 startColor;
	glm::vec4 endColor;

	vector<Particle::Vertex> vertices;

	RenderData renderData;
	ShaderProgram shaderProgram;

	void Init(uint maxParticles,
		uint emitRate,
		float minLifetime,
		float maxLifetime,
		float minVelocity,
		float maxVelocity,
		float startSize,
		float endSize,
		const glm::vec4& startColor,
		const glm::vec4& endColor);

	void InitShaderProgram();

	void Emit();

	void Update(float deltaTime, const glm::mat4& cameraTransform);

	void Draw();
};

