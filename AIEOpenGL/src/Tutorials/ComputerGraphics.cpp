#include "ComputerGraphics.h"
#include "..\gl_core_4_4.h"

#include "..\Camera.h"
#include "FBXFile.h"
#include "..\Gizmos.h"
#include "..\ImGUI\imgui.h"
#include "..\ShaderProgram.h"


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using glm::vec3;
using glm::vec4;
using glm::mat4;

ComputerGraphics::ComputerGraphics() :
    m_lightDir(0,1,0)
{

}

ComputerGraphics::~ComputerGraphics() {

}

bool ComputerGraphics::startup() {
	srand((uint)time(NULL));

    // create a basic window
    createWindow("Tutorial - ComputerGraphics", 1024, 768);

	lightColor = vec3(1, 0, 0);
	soulspear.Load("soulspear.fbx");
	planet.Load("AlienPlanet2.fbx");

	soulspear.transform.position = vec3(0, 5, 0);
	planet.transform.position = vec3(0, 5, 5);
	soulspear.transform.rotation.AddY(90);

	emitter.Init(1000, 500,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));


	emitter.InitShaderProgram();
	terrain.Create();
	terrain.InitShaderProgram();


	elapsedTime = 0;

    return true;
}

void ComputerGraphics::shutdown() {

	soulspear.GLDelete();
	planet.GLDelete();
	emitter.shaderProgram.GLDelete();

    // destroy our window properly
    destroyWindow();
}

bool ComputerGraphics::update(float deltaTime) {

    // close the application if the window closes
    if (glfwWindowShouldClose(m_window) ||
        glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return false;

    // update the camera's movement
    m_camera->update(deltaTime);
	elapsedTime += deltaTime;

	emitter.position += vec3(glm::sin(elapsedTime), 0, 0);

	emitter.Update(deltaTime, m_camera->getTransform());
    // clear the gizmos out for this frame
    Gizmos::clear();
	terrain.Update(deltaTime);

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

void ComputerGraphics::drawGUI()
{
	ImGui::DragFloat3("Spear Position", glm::value_ptr(soulspear.transform.position));
	ImGui::ColorEdit3("Light Direction", glm::value_ptr(m_lightDir));
}


void ComputerGraphics::draw() {
	Clear();
	terrain.material.Init(terrain.shaderProgram);
	terrain.Draw(*m_camera);

	
	emitter.shaderProgram.Use();
	emitter.shaderProgram.SetUniform("projectionView", m_camera->getProjectionView());
	emitter.Draw();
	soulspear.Draw(*m_camera, m_lightDir, lightColor);
	planet.Draw(*m_camera, m_lightDir, lightColor);


    // display the 3D gizmos
    Gizmos::draw(m_camera->getProjectionView());
}



