#pragma once

#include "..\BaseApplication.h"
#include "..\ShaderProgram.h"
#include "..\RenderData.h"
#include "..\TexturedGrid.h"

class Camera;

class RenderingGeometry : public BaseApplication {
public:
	virtual bool startup() override;
	virtual void shutdown() override;

	virtual bool update(float deltaTime) override;
	virtual void draw() override;

private:
    void generateGrid(unsigned int rows, unsigned int cols);

    unsigned int m_programID;

	ShaderProgram shaderProgram;
	RenderData renderer;

	TexturedGrid grid;

    int m_rows;
    int m_cols;

    float m_timer = 0;
};