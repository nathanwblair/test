#include "FBXLighting.h"
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

FBXLighting::FBXLighting() :
    m_lightDir(0,1,0)
{

}

FBXLighting::~FBXLighting() {

}

bool FBXLighting::startup() {
	srand(time(NULL));

    // create a basic window
    createWindow("Tutorial - FBXLighting", 1024, 768);

	lightColor = vec3(1, 0, 0);
 //   m_fbx = new FBXFile();
	//m_fbx->load("soulspear.fbx");//"./data/models/stanford/Bunny.fbx");
 //   createOpenGLBuffers(m_fbx);
	mesh.Load("soulspear.fbx");

	emitter.Init(1000, 500,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));


	//shaderProgram.Load("data/shaders/fbx_lighting.shader");

    //m_programID = shaderProgram.id;


	emitter.InitShaderProgram();
	terrain.Create();
	terrain.InitShaderProgram();


	elapsedTime = 0;

    return true;
}

void FBXLighting::shutdown() {
    //cleanupOpenGLBuffers(m_fbx);

	//shaderProgram.GLDelete();
	mesh.GLDelete();
	emitter.shaderProgram.GLDelete();

    // destroy our window properly
    destroyWindow();
}

bool FBXLighting::update(float deltaTime) {

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

void FBXLighting::drawGUI()
{
    ImGui::ColorEdit3("Light Direction", glm::value_ptr(m_lightDir));
}


void FBXLighting::draw() {
	Clear();

	//shaderProgram.Use();
	terrain.material.Init(terrain.shaderProgram);
	terrain.Draw(*m_camera);
    // bind the camera
 //   shaderProgram.SetUniform("ProjectionView", m_camera->getProjectionView());
	//shaderProgram.SetUniform("LightDir", m_lightDir);
	//shaderProgram.SetUniform("LightColour", glm::vec3(1, 0, 0));
	//shaderProgram.SetUniform("CameraPos", glm::vec3(m_camera->getTransform()[3]));
	//shaderProgram.SetUniform("SpecPow", 5.0f);

 //   // bind our vertex array object and draw the mesh
 //   for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {

 //       FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);

 //       unsigned int* glData = (uint*)mesh->m_userData;

 //       glBindVertexArray(glData[0]);
 //       glDrawElements(GL_TRIANGLES,
 //           (uint)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
 //   }

	
	emitter.shaderProgram.Use();
	emitter.shaderProgram.SetUniform("projectionView", m_camera->getProjectionView());
	emitter.Draw();
	mesh.Draw(*m_camera, m_lightDir, lightColor);

	// unbind
	//glBindVertexArray(0);
	//shaderProgram.Unbind();


    // display the 3D gizmos
    Gizmos::draw(m_camera->getProjectionView());
}

void FBXLighting::createOpenGLBuffers(FBXFile* fbx)
{
  //  // create the GL VAO/VBO/IBO data for each mesh
  //  for (uint index = 0; index < fbx->getMeshCount(); ++index)
  //  {
  //      FBXMeshNode* mesh = fbx->getMeshByIndex(index);

  //      // storage for the opengl data in 3 unsigned int
  //      //unsigned int* glData = new unsigned int[3];

		//renderer.GenerateBuffers(RenderData::Buffers::ALL);
		//renderer.Bind();

  //      //glGenVertexArrays(1, &glData[0]);
  //     // glBindVertexArray(glData[0]);

  //      //glGenBuffers(1, &glData[1]);
  //      //glGenBuffers(1, &glData[2]);

  //      //glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
  //      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

  //      glBufferData(GL_ARRAY_BUFFER,
  //          mesh->m_vertices.size() * sizeof(FBXVertex),
  //          mesh->m_vertices.data(), GL_STATIC_DRAW);
  //      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
  //          mesh->m_indices.size() * sizeof(unsigned int),
  //          mesh->m_indices.data(), GL_STATIC_DRAW);

  //      glEnableVertexAttribArray(0); // position
  //      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
  //          sizeof(FBXVertex), 0);

		//glEnableVertexAttribArray(1); // normal
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
		//	sizeof(FBXVertex),
		//	((char*)0) + FBXVertex::NormalOffset);

		//renderer.Unbind();
  //      //glBindVertexArray(0);
  //      //glBindBuffer(GL_ARRAY_BUFFER, 0);
  //      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //      mesh->m_userData = renderer.CreateRaw();
  //  }
}

void FBXLighting::cleanupOpenGLBuffers(FBXFile* fbx) {

    //// clean up the vertex data attached to each mesh
    //for (unsigned int i = 0; i < fbx->getMeshCount(); ++i) {
    //    FBXMeshNode* mesh = fbx->getMeshByIndex(i);

    //    unsigned int* glData = (unsigned int*)mesh->m_userData;

    //    glDeleteVertexArrays(1, &glData[0]);
    //    glDeleteBuffers(1, &glData[1]);
    //    glDeleteBuffers(1, &glData[2]);

    //    delete[] glData;
    //}

	//shaderProgram.GLDelete();
}



