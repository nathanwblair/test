#include "AdvancedTexturing.h"
#include "gl_core_4_4.h"

#include "Camera.h"
#include "Gizmos.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "stb_image.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

AdvancedTexturing::AdvancedTexturing() {

}

AdvancedTexturing::~AdvancedTexturing() {

}

bool AdvancedTexturing::startup() {
	// create a basic window
	createWindow("Tutorial - AdvancedTexturing", 1024, 768);


	light = vec3(sin(glfwGetTime()), 1, cos(glfwGetTime()));


	material.Add("diffuse", "./data/textures/four_diffuse.tga");
	material.Add("normal", "./data/textures/four_normal.tga");

    //struct Vertex {
    //    float x, y, z, w;
    //    float nx, ny, nz, nw;
    //    float tx, ty, tz, tw;
    //    float s, t;
    //};
	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec2 textureCoord;
	};

	vec4 normal(0, 1, 0, 0);
	vec4 tangent(1, 0, 0, 0);

	Vertex vertexTemplate = { vec4(0), normal, tangent };

	vec4 vertexPositionData[] = {
		{ -5, 0, 5, 1 },
		{ 5, 0, 5, 1 },
		{ 5, 0, -5, 1 },
		{ -5, 0, -5, 1 },
	};

	glm::vec2 vertexTextureCoordinateData[] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },
	};

	Vertex vertexData[4];

	for (uint i = 0; i < 4; i++)
	{
		vertexData[i].position = vertexPositionData[i];
		vertexData[i].normal = normal;
		vertexData[i].tangent = tangent;
		vertexData[i].textureCoord = vertexTextureCoordinateData[i];
	}

    unsigned int indexData[] = {
        0, 1, 2,
        0, 2, 3,
    };

	renderer.GenerateBuffers(RenderData::ALL);
	renderer.Bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4,
        vertexData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6,
        indexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), ((char*)0) + 48);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), ((char*)0) + 16);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), ((char*)0) + 32);

	renderer.Unbind();

	shaderProgram.Load("./data/shaders/textured_grid.shader");

	m_program = shaderProgram.id;

	material.Init(shaderProgram);

	return true;
}

void AdvancedTexturing::shutdown() {
	// destroy our window properly
	destroyWindow();
}

bool AdvancedTexturing::update(float deltaTime) {
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

    // clear the gizmos out for this frame
    Gizmos::clear();

	// update the camera's movement
	m_camera->update(deltaTime);

	light = vec3(sin(glfwGetTime()), 1, cos(glfwGetTime()));

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

void AdvancedTexturing::draw() {

	// clear the screen for this frame
	Clear();

    // use our texture program
	shaderProgram.Use();

	// bind the camera
	shaderProgram.SetUniform("ProjectionView", m_camera->getProjectionView());

	// bind the light
	shaderProgram.SetUniform("LightDir", light);

    // draw
    glBindVertexArray(renderer.GetVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());
}