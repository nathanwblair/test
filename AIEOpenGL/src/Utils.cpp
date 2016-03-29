#include "Utils.h"

#include <fstream>
#include <sstream>

#include <glm\gtc\noise.hpp>

#define MAKE_CASE(slot_num) case slot_num: \
		return GL_TEXTURE##slot_num

Application *GetApplication(Application * _application) {
	static Application * application = nullptr;

	if (_application != nullptr)
	{
		application = _application;
	}

	return application;
}

uint GL::SizeOf(Primitive primitiveType) {
	switch (primitiveType)
	{
		case GL::Primitive::Double:
			return sizeof(double);
		case GL::Primitive::Float:
			return sizeof(float);
		case GL::Primitive::Int:
			return sizeof(int);
		case GL::Primitive::UInt:
			return sizeof(uint);
		case GL::Primitive::Short:
			return sizeof(short);
		case GL::Primitive::Byte:
			return sizeof(char);
		case GL::Primitive::UByte:
			return sizeof(uchar);
	}
}


string FromFile(string path)
{
	std::ifstream file;
	file.open(path);
	assert(file.good() && "Unable to open text file");

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	return ss.str();
}

GLenum ToGLSlot(int slot)
{
	switch (slot)
	{
		MAKE_CASE(0);
		MAKE_CASE(1);
		MAKE_CASE(2);
		MAKE_CASE(3);
		MAKE_CASE(4);
		MAKE_CASE(5);
		MAKE_CASE(6);
		MAKE_CASE(7);
		MAKE_CASE(8);
		MAKE_CASE(9);
		MAKE_CASE(10);
		MAKE_CASE(11);
		MAKE_CASE(12);
		MAKE_CASE(13);
		MAKE_CASE(14);
		MAKE_CASE(15);
		MAKE_CASE(16);
		MAKE_CASE(17);
		MAKE_CASE(18);
		MAKE_CASE(19);
		MAKE_CASE(20);
		MAKE_CASE(21);
		MAKE_CASE(22);
		MAKE_CASE(23);
		MAKE_CASE(24);
		MAKE_CASE(25);
		MAKE_CASE(26);
		MAKE_CASE(27);
		MAKE_CASE(28);
		MAKE_CASE(29);
		MAKE_CASE(30);
		MAKE_CASE(31);
	default:
		break;
	}
}

vector<float> GeneratePerlin(uint length)
{
	vector<float> perlinData;
	perlinData.resize(length * length);

	auto octaves = 6;

	auto scale = (1.0f / length) * 3;

	for (int x = 0; x < (int)length; ++x)
	{
		for (int y = 0; y < (int)length; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinData[y * length + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;

				perlinData[y * length + x] += perlin_sample * amplitude;

				amplitude *= persistence;
			}
		}
	}

	return perlinData;
}