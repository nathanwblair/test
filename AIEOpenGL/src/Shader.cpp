#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(ShaderType _type)
	: type(_type)
{
}


Shader::~Shader()
{
}


void Shader::SetType(ShaderType _type)
{
	type = _type;
}


void Shader::Create()
{
	id = glCreateShader((GLenum)type);
}


void Shader::Load(string path)
{
	location = path;

	Create();

	std::ifstream file;
	assert(file.good() && "Unable to open shader file");

	auto shaderCode = FromFile(path);
	const auto pCode = shaderCode.c_str();

	glShaderSource(id, 1, &pCode, 0);
	glCompileShader(id);

	loaded = CheckGLErrors();
}

void Shader::Load(string path, std::function<void(int)> initFeedback)
{
	Load(path);

	initFeedback(id);
}

bool Shader::CheckGLErrors()
{
	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) 
	{
		auto logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		auto logBuffer = new char[logLength];
		glGetShaderInfoLog(id, logLength, NULL, logBuffer);
		delete[] logBuffer;

		return false;
	}

	return true;
}


void Shader::GLDelete()
{
	glDeleteShader(id);
}


void Shader::Unload()
{

}
