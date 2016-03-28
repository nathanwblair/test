#pragma once
#include <string>

#include "Utils.h"

class Asset
{
public:
	bool loaded;
	string location;

	Asset();
	~Asset();

	virtual void Load(string location)
	{
		// Do nothing
	};
	virtual void Unload()
	{
		// Do nothing
	};
};

