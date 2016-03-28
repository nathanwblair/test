#pragma once
#include "Grid.h"

#include "Texture.h"
#include "Material.h"

class TexturedGrid :
	public Grid
{
public:
	TexturedGrid();
	~TexturedGrid();

	void Create() override;
	void InitShaderProgram(string name="grid") override;
	void GeneratePerlin();

	Material material;
};