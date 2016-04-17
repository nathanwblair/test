#include "GLAsset.h"

#include <string>

#include "Shader.h"

#pragma once
class ShaderProgram 
	: public GLAsset
{
private:
	void Link();
	void Create(); 
	void Attach(Shader& shader);

public:
	ShaderProgram();
	~ShaderProgram();

	void Use();
	void Unbind();

	int GetUniformLocation(string name);

#pragma region SetUniforms

	void SetUniform(string name, uint value);
	void SetUniform(string name, int value);
	void SetUniform(string name, float value);
	void SetUniform(string name, const glm::mat4& value, int count=1);
	void SetUniform(string name, const glm::vec3& value, int count=1);
	void SetUniform(string name, const glm::vec4& value, int count=1);

#pragma endregion

	void Load(string path) override;
	void Load(string path, std::function<void(int)> initFeedback);
	void Unload() override;

	void GLDelete() override;
	bool CheckGLErrors() override;

	Shader geometryShader;
	Shader vertexShader;
	Shader fragmentShader;
};

