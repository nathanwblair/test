#pragma once
#include "GLAsset.h"

class VertexBufferObject :
	public GLAsset
{
public:
	VertexBufferObject();
	~VertexBufferObject();

	void GLDelete() override;

	void Create() override;

	void Use() override;
	void Unbind() override;
};

