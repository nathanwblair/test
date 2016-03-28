#pragma once
#include "GLAsset.h"

#include "Texture.h"
#include "ShaderProgram.h"

#include <unordered_map>
#include <FBXFile.h>

class Material :
	public GLAsset
{
public:
	std::unordered_map<string, Texture*> textures;

	Material();
	~Material();

	bool CheckGLErrors() override;

	void GLDelete() override;

	void Add(string name, Texture * texture);
	void Add(string name, string location);
	void Add(string name, uchar* data, int width, int height);

	void Load(FBXMaterial * material);

	void Init(ShaderProgram& shaderProgram);

	void Create();
	void CreateSlots();

	void SetSlotUniforms(ShaderProgram& shaderProgram);
};

