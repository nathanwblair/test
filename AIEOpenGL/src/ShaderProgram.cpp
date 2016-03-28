#include "ShaderProgram.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

ShaderProgram::ShaderProgram()
	: vertexShader(Shader::ShaderType::Vertex),
	fragmentShader(Shader::ShaderType::Fragment)
{
}


ShaderProgram::~ShaderProgram()
{
}


void ShaderProgram::Load(string path)
{
	location = path;

	assert(!loaded && "Already have loaded a shader");

	std::ifstream file;
	file.open(path, std::ifstream::in);

	assert(file.good() && "Failed to open shader file!");

	string programPath = path.substr(0, path.find_last_of('/') + 1);

	while (file.good())
	{
		string line = "";
		std::getline(file, line);

		if (line == "")
			break;

		auto shaderPath = programPath + line.substr(5);
		auto shaderToLoad = line.substr(0, 4);

		if (shaderToLoad == "vert")
		{
			vertexShader.Load(shaderPath);
		}
		else if (shaderToLoad == "frag")
		{
			fragmentShader.Load(shaderPath);
		}
	}

	Create();
	Attach(vertexShader);
	Attach(fragmentShader);
	Link();

	CheckGLErrors();

	vertexShader.GLDelete();
	fragmentShader.GLDelete();
}


void ShaderProgram::Link()
{
	glLinkProgram(id);
}


void ShaderProgram::Create()
{
	id = glCreateProgram();
}


void ShaderProgram::Attach(Shader& shader)
{
	glAttachShader(id, shader.id);
}


void ShaderProgram::Use()
{
	glUseProgram(id);
}


void ShaderProgram::Unbind()
{
	glUseProgram(0);
}


bool ShaderProgram::CheckGLErrors() 
{
	auto success = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		auto infoLogLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = new char[infoLogLength];
		glGetProgramInfoLog(id, infoLogLength, 0, infoLog);

		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);

		delete[] infoLog;

		return false;
	}

	return true;
}


void ShaderProgram::Unload()
{

}


void ShaderProgram::GLDelete()
{
	glDeleteProgram(id);
}


int ShaderProgram::GetUniformLocation(string name)
{
	return glGetUniformLocation(id, name.c_str());
}


#pragma region SetUniforms

void ShaderProgram::SetUniform(string name, uint value)
{
	glUniform1ui(GetUniformLocation(name), value);
}


void ShaderProgram::SetUniform(string name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}


void ShaderProgram::SetUniform(string name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}


void ShaderProgram::SetUniform(string name, const glm::mat4& value, int count)
{
	glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, glm::value_ptr(value));
}


void ShaderProgram::SetUniform(string name, const glm::vec3& value, int count)
{
	glUniform3fv(GetUniformLocation(name), count, glm::value_ptr(value));
}


void ShaderProgram::SetUniform(string name, const glm::vec4& value, int count)
{
	glUniform4fv(GetUniformLocation(name), count, glm::value_ptr(value));
}

#pragma endregion