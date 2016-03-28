#include "TexturedGrid.h"

#include <glm\gtc\noise.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

TexturedGrid::TexturedGrid()
{
}


TexturedGrid::~TexturedGrid()
{
}


void TexturedGrid::InitShaderProgram(string name)
{
	Grid::InitShaderProgram("textured_grid");

	material.Init(shaderProgram);
}


void TexturedGrid::Create()
{
	rows = 20;
	columns = 20;
	Grid::Create();

	material.Add("diffuse", "./data/textures/four_diffuse.tga");
	GeneratePerlin();
}


void TexturedGrid::GeneratePerlin()
{
	assert(rows == columns);
	uint length = rows;

	vector<float> perlinData;
	perlinData.resize(length * length);	
	
	auto octaves = 6;

	auto scale = (1.0f / length) * 3;

	for (int x = 0; x < length; ++x)
	{
		for (int y = 0; y < length; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinData[y * length + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;

				perlinData[y * length + x] += perlin_sample * amplitude;

				amplitude *= persistence;
			}
		}
	}

	auto perlinTexture = new Texture();

	perlinTexture->Init(perlinData, length, length);
	
	material.Add("perlinTexture", perlinTexture);
}




