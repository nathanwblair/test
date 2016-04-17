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

bool RenderingGeometry::startup() 
{
	// create a basic window
	createWindow("Tutorial - Rendering Geometry", 1024, 768);

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