#pragma once
#include "Asset.h"

class GLAsset :
	public Asset
{
public:
	uint id;

	GLAsset();
	~GLAsset();

	virtual void Load(string location = "") override
	{
		// Do nothing
	};

	virtual bool CheckGLErrors() = 0;
	virtual void GLDelete() = 0;

	virtual void Create()
	{

	}
	virtual void Use() 
	{
		// Do nothing
	}
	virtual void Unbind()
	{
		// Do nothing
	}
};

