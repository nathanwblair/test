#include "GLAsset.h"
#include <string>
#include <functional>

#pragma once
class Shader 
	: public GLAsset
{
public:
	enum class ShaderType
	{
		None,
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER
	};

	ShaderType type;
	string sourceCode;

	Shader(ShaderType type=ShaderType::None);
	~Shader();

	void Load(string path) override;
	void Load(string path, std::function<void(int)> initFeedback);
	void Unload() override;

	void SetType(ShaderType type);

	void Create();

	bool CheckGLErrors() override;
	void GLDelete() override;
};

