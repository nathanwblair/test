#include "RenderingGeometry.h"
#include "..\gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "..\Camera.h"
#include "..\Gizmos.h"

#include "..\Utils.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex {
    vec4 position;
	vec4 color;
	vec2 uv;
};

void RenderingGeometry::generateGrid(uint rows, uint cols)
{
    m_rows = rows;
    m_cols = cols;

    Vertex* aoVertices = new Vertex[rows * cols];
    for (uint r = 0; r < rows; ++r) {
        for (uint c = 0; c < cols; ++c) {
			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);
			
			vec2 uv;
			uv.x = (float)c / (float)cols;
			uv.y = 1.0f - ((float)r / (float)rows);

			aoVertices[r * cols + c].uv = uv;

            // create some arbitrary colour based off something 
            // that might not be related to tiling a texture
            vec3 colour = vec3(sinf((c / (float)(cols - 1)) *
                (r / (float)(rows - 1))));
            aoVertices[r * cols + c].color = vec4(colour, 1);
        }
    }

    // defining index count based off quad count (2 triangles per quad)
    uint* auiIndices = new uint[(rows - 1) * (cols - 1) * 6];

    uint index = 0;
    for (uint r = 0; r < (rows - 1); ++r) {
        for (uint c = 0; c < (cols - 1); ++c) {
            // triangle 1
            auiIndices[index++] = r * cols + c;
            auiIndices[index++] = (r + 1) * cols + c;
            auiIndices[index++] = (r + 1) * cols + (c + 1);

            // triangle 2
            auiIndices[index++] = r * cols + c;
            auiIndices[index++] = (r + 1) * cols + (c + 1);
            auiIndices[index++] = r * cols + (c + 1);
        }
    }

	renderer.GenerateBuffers(RenderData::ALL);

	renderer.Bind();

    glBufferData(GL_ARRAY_BUFFER, 
		(rows * cols) * sizeof(Vertex),
        aoVertices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		(rows - 1) * (cols - 1) * 6 * sizeof(uint), 
		auiIndices, 
		GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	renderer.Unbind();

    delete[] aoVertices;
}

bool RenderingGeometry::startup() 
{
	// create a basic window
	createWindow("Tutorial - Rendering Geometry", 1024, 768);

    //generateGrid(10, 10);

	//shaderProgram.Load("data/shaders/grid.shader");

	//m_programID = shaderProgram.id;

	grid.Create();
	grid.InitShaderProgram();

	return true;
}

void RenderingGeometry::shutdown() 
{
	// destroy our window properly
	destroyWindow();
}

bool RenderingGeometry::update(float deltaTime) 
{
    m_timer += deltaTime;
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);
	grid.Update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));

	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}

	// return true, else the application closes
	return true;
}

void RenderingGeometry::draw() {

	// clear the screen for this frame
	Clear();

	grid.Draw(*m_camera);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());
}