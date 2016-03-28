#pragma once

#include "Utils.h"

class RenderData
{
private:
	uint vao;
	uint vbo;
	uint ibo;

	uint uiIndexCount;

	void PipePositive(uchar& output, uint positive, uchar pipeValue);

public:
	RenderData();

	~RenderData();

	RenderData(const RenderData& other);

	RenderData(RenderData&& other);

	enum Buffers : uchar
	{
		VAO = 1,
		VBO = 2,
		IBO = 4,
		ALL = 7
	};

	void SetIndexCount(uint count);

	void GenerateBuffers(uchar uiBuffers);

	void DestroyBuffers(uchar uiBuffers);

	uint GetVAO() const;

	uint GetVBO() const;

	uint GetIBO() const;

	uint * CreateRaw() const;

	void CheckBuffers(uchar& uiBuffers);

	void Bind();

	void Render();

	void Clear();

	void Unbind();
};