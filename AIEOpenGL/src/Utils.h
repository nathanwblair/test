#pragma once
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#define GLM_SWIZZLE_XYZW 
#include "glm/glm.hpp"
#include <vector>
#include <string>

#define MAX_TEXTURE_SLOT 31

class Application;

using std::string;
using std::vector;

typedef unsigned int uint;
typedef unsigned char uchar;

#define NAME(type) typeid(type).name()

struct GL
{
	enum class Primitive
	{
		Double = GL_DOUBLE,
		Float = GL_FLOAT,
		Int = GL_INT,
		UInt = GL_UNSIGNED_INT,
		Short = GL_SHORT,
		Byte = GL_BYTE,
		UByte = GL_UNSIGNED_BYTE
	};

	static uint SizeOf(Primitive primitiveType);
};

vector<float> GeneratePerlin(uint length);

string FromFile(string path);

GLenum ToGLSlot(int texture_slot);

#pragma region Vertex Utils

static uint VAO_NUM_FLOATS;
static uint VAO_NUM_ATTRIBUTES;

#define subscribe(vertex_t, attribute_name) \
{ \
	auto offset = ((size_t)&reinterpret_cast<char const volatile&>((((vertex_t*)0)->attribute_name))); \
	PushAttribute(sizeof(vertex_t::attribute_name) / sizeof(float), sizeof(vertex_t), offset);\
}

void PushAttribute(uint attributeSize, uint vertexSize, size_t offset);
void ClearVAOMetadata();

#pragma endregion