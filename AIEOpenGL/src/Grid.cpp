#include "Grid.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


Grid::Grid(uint _rows, uint _columns, float _heightScale)
	: timer(0),
	rows(_rows),
	columns(_columns),
	heightScale(_heightScale)
{
}


Grid::~Grid()
{
}


void Grid::EnableAttributes()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}


void Grid::Create()
{
	Vertex* aoVertices = new Vertex[rows * columns];
	for (uint r = 0; r < rows; ++r) {
		for (uint c = 0; c < columns; ++c) {
			aoVertices[r * columns + c].position = vec4(
				(float)c, 0, (float)r, 1);

			vec2 uv;
			uv.x = (float)c / (float)columns;
			uv.y = 1.0f - ((float)r / (float)rows);
			
			aoVertices[r * columns + c].uv = uv;

			// create some arbitrary colour based off something 
			// that might not be related to tiling a texture
			vec3 colour = vec3(sinf((c / (float)(columns - 1)) *
				(r / (float)(rows - 1))));
			aoVertices[r * columns + c].color = vec4(colour, 1);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	uint* auiIndices = new uint[(rows - 1) * (columns - 1) * 6];

	uint index = 0;
	for (uint r = 0; r < (rows - 1); ++r) {
		for (uint c = 0; c < (columns - 1); ++c) {
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


void Grid::Update(float deltaTime)
{
	timer += deltaTime;
}


void Grid::Draw(const Camera& camera)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shaderProgram.Use();
	renderer.Bind();
	
	shaderProgram.SetUniform("ProjectionView", camera.getProjectionView());
	shaderProgram.SetUniform("time", timer);
	shaderProgram.SetUniform("heightScale", 1.0f);

	uint indexCount = (rows - 1) * (columns - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	renderer.Unbind();
	shaderProgram.Unbind();
}


void Grid::InitShaderProgram(string name)
{
	shaderProgram.Load("data/shaders/" + name + ".shader");
}