#include "GLAsset.h"
#include <string>

#pragma once
class Shader 
	: public GLAsset
{
public:
	enum class ShaderType
	{
		None,
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	ShaderType type;
	string sourceCode;

	Shader(ShaderType type=ShaderType::None);
	~Shader();

	void Load(string path) override;
	void Unload() override;

	void SetType(ShaderType type);

	void Create();

	bool CheckGLErrors() override;
	void GLDelete() override;
};

