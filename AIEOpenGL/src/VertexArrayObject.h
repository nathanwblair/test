#pragma once
#include "GLAsset.h"
class VertexArrayObject :
	public GLAsset
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void GLDelete() override;

	void Create() override;

	void Use() override;
	void Unbind() override;
};

