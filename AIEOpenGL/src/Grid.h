#pragma once
#include "GLAsset.h"
#include "ShaderProgram.h"
#include "RenderData.h"
#include "Camera.h"

class Grid :
	public GLAsset
{
public:

	struct Vertex 
	{
		glm::vec4 position;
		glm::vec4 color;
		glm::vec2 uv;
		glm::vec4 normal;
		glm::vec3 tangnet;
	};

	Grid(uint _rows = 10, uint _columns = 10, float heightScale = 1.0f);
	~Grid();
	ShaderProgram shaderProgram;
	RenderData renderer;

	float timer;

	uint rows;
	uint columns;
	float heightScale;

	virtual void InitShaderProgram(string name="grid");

	void Update(float deltaTime);
	void Draw(const Camera& camera);
	virtual void Create() override;

	virtual void EnableAttributes();

	void GLDelete() override {}
	bool CheckGLErrors() override { return true; }
};

