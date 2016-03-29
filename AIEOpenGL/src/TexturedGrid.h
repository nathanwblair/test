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
	void Create(const vector<float>& heightMap);
	void InitShaderProgram(string name="grid") override;

	Material material;
};