#include "Texture.h"


#include "stb_image.h"

Texture::Texture() : hasAlpha(false)
{
}


Texture::~Texture()
{
}


void Texture::Load(string path)
{
	auto format = 0;
	unsigned char* _data;

	auto data_format = STBI_default;

	if (path.substr(path.find_last_of('.')) == ".png")
	{
		data_format = STBI_rgb_alpha;
	}

	// load diffuse map
	_data = stbi_load((path).c_str(),
		&width,
		&height,
		&format,
		data_format);

	Create();

	if (format == 4)
	{
		PushRGBAData(_data);
		hasAlpha = true;
	}
	else
	{
		PushData(_data);
	}

	stbi_image_free(_data);
}

void Texture::Load(uchar* _data, int width, int height)
{
	Create();

	PushData(_data);
}


void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, id);
}


void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Create()
{
	glGenTextures(1, &id);
}


void Texture::Init(const vector<float>& _data, uint _width, uint _height)
{
	data = _data;
	width = _width;
	height = _height;

	Init();
}


void Texture::Init()
{
	Create();

	PushData();
}


void Texture::PushData()
{
	Use();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	Unbind();

	data.clear();
}

void Texture::PushData(uchar* _data)
{
	Use();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Unbind();

	data.clear();
}

void Texture::PushRGBAData(uchar* _data)
{
	Use();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Unbind();

	data.clear();
}


void Texture::GLDelete()
{
}


bool Texture::CheckGLErrors()
{
	// TODO

	return true;
}