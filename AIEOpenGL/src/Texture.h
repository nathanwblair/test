#pragma once
#include "GLAsset.h"
class Texture :
	public GLAsset
{
public:
	bool hasAlpha;

	Texture();
	~Texture();

	int width;
	int height;

	void Use() override;
	void Unbind() override;

	void GLDelete() override;

	void Create() override;

	void Load(string path = "") override;
	void Load(uchar* _data, int width, int height);

	void Init(const vector<float>& _data, uint width = 1, uint height = 1);
	void Init(const uchar* _data, uint width = 1, uint height = 1);

	void Init();

	void PushData();
	void PushData(uchar* data);
	void PushRGBAData(uchar* data);

	bool CheckGLErrors() override;
	
	vector<float> data;
};

