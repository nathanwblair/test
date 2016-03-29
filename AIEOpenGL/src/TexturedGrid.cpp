#include "Utils.h"
#include "TexturedGrid.h"

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

	material.Add("diffuse", "./data/textures/four_diffuse.tga");
	material.Add("normal", "./data/textures/four_normal.tga");
	auto perlinData = GeneratePerlin(rows);

	Create(perlinData);
	auto perlinTexture = new Texture();

	perlinTexture->Init(perlinData, rows, columns);

	material.Add("perlinTexture", perlinTexture);
}




void TexturedGrid::Create(const vector<float>& heightMap)
{
	Vertex* aoVertices = new Vertex[rows * columns];
	for (uint r = 0; r < rows; ++r) 
	{
		for (uint c = 0; c < columns; ++c) 
		{
			auto index = r * columns + c;

			float potentialHeight = heightMap[index];
			vec3 potentialPos((float)c, potentialHeight, (float)r);

			aoVertices[index].position = vec4(potentialPos, 1);

			vec2 uv;
			uv.x = (float)c / (float)columns;
			uv.y = 1.0f - ((float)r / (float)rows);

			aoVertices[index].uv = uv;

			// create some arbitrary colour based off something 
			// that might not be related to tiling a texture
			vec3 colour = vec3(sinf((c / (float)(columns - 1)) *
				(r / (float)(rows - 1))));
			aoVertices[index].color = vec4(colour, 1);
		}
	}

	for (uint r = 0; r < rows; ++r)
	{
		for (uint c = 0; c < columns; ++c)
		{
			auto index = r * columns + c;

			auto offset = vec2(-1, -1);
			if (c == 0)
				offset.y = 1;
			if (r == 0)
				offset.x = 1;

			auto prevIndex = r * columns + c + (int)offset.x;
			auto nextIndex = (r + (int)offset.y) * columns + c;

			vec3 prevPos = vec3(aoVertices[prevIndex].position);
			vec3 nextPos = vec3(aoVertices[nextIndex].position);
			vec3 pos = vec3(aoVertices[index].position);

			vec2 prevUV = aoVertices[prevIndex].uv;
			vec2 nextUV = aoVertices[nextIndex].uv;
			vec2 uv = aoVertices[index].uv;


			// Edges of the triangle, postion delta
			glm::vec3 deltaPos1 = nextPos - pos;
			glm::vec3 deltaPos2 = prevPos - pos;

			// UV delta
			glm::vec2 deltaUV1 = nextUV - uv;
			glm::vec2 deltaUV2 = prevUV - uv;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;

			aoVertices[index].tangnet = tangent;

			aoVertices[index].normal = vec4(glm::normalize(glm::cross(prevPos - pos, nextPos - pos)), 0);
		}
	}


	for (uint r = 0; r < rows; ++r)
	{
		for (uint c = 0; c < columns; ++c)
		{
			auto index = r * columns + c;

			aoVertices[index].position.y = 0;
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	uint* auiIndices = new uint[(rows - 1) * (columns - 1) * 6];

	uint index = 0;
	for (uint r = 0; r < (rows - 1); ++r) 
	{
		for (uint c = 0; c < (columns - 1); ++c)
		{
			// triangle 1
			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);

			// triangle 2
			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			auiIndices[index++] = r * columns + (c + 1);
		}
	}

	renderer.GenerateBuffers(RenderData::ALL);

	renderer.Bind();

	glBufferData(GL_ARRAY_BUFFER,
		(rows * columns) * sizeof(Vertex),
		aoVertices, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		(rows - 1) * (columns - 1) * 6 * sizeof(uint),
		auiIndices,
		GL_STATIC_DRAW);

	EnableAttributes();

	renderer.Unbind();

	delete[] aoVertices;
}


