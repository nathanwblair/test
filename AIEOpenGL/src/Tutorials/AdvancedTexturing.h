#pragma once

#include "BaseApplication.h"

#include "..\RenderData.h"
#include "..\ShaderProgram.h"
#include "..\Texture.h"
#include "..\Material.h"

class Camera;

class AdvancedTexturing : public BaseApplication {
public:

    AdvancedTexturing();
    virtual ~AdvancedTexturing();

    virtual bool startup() override;
    virtual void shutdown() override;

    virtual bool update(float deltaTime) override;
    virtual void draw() override;

private:
    unsigned int m_texture, m_normalmap;

	int m_program;

	ShaderProgram shaderProgram;
	RenderData renderer;
	Texture diffuse;
	Texture normal;

	Material material;

	glm::vec3 light;

};