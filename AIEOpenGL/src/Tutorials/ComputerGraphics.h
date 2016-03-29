#pragma once

#include "..\BaseApplication.h"
#include "..\ShaderProgram.h"
#include "..\RenderData.h"
#include "..\ParticleEmitter.h"
#include "..\Mesh.h"
#include "..\TexturedGrid.h"

#include <glm/vec3.hpp>

class Camera;
class FBXFile;

class ComputerGraphics : public BaseApplication 
{
public:
    ComputerGraphics();
    virtual ~ComputerGraphics();

    virtual bool startup() override;
    virtual void shutdown() override;

    virtual bool update(float deltaTime) override;
    virtual void draw() override;
    virtual void drawGUI() override;

private:
    void createOpenGLBuffers(FBXFile* fbx);
    void cleanupOpenGLBuffers(FBXFile* fbx);

 //   FBXFile* m_fbx;
 //   unsigned int m_programID;

	//ShaderProgram shaderProgram;
	//RenderData renderer;


	Mesh soulspear;
	Mesh planet;
	TexturedGrid terrain;

	glm::vec3 lightColor;

	ParticleEmitter emitter;

	float elapsedTime;

    glm::vec3 m_lightDir;
};