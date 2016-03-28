#include "RenderData.h"


void RenderData::PipePositive(uchar& output, uint positive, uchar pipeValue)
{
	if (positive > 0)
		output |= pipeValue;
}


RenderData::RenderData()
	: vao(0),
	vbo(0),
	ibo(0),
	uiIndexCount(0)
{
}


RenderData::~RenderData()
{
	DestroyBuffers(Buffers::ALL);
}


RenderData::RenderData(const RenderData& other)
{
	vao = other.vao;
	vbo = other.vbo;
	ibo = other.ibo;
}

\
RenderData::RenderData(RenderData&& other)
{
	vao = other.vao;
	vbo = other.vbo;
	ibo = other.ibo;

	other.Clear();
}


uint * RenderData::CreateRaw() const
{
	auto result = new uint[3];

	result[0] = vao;
	result[1] = vbo;
	result[2] = ibo;

	return result;
}


void RenderData::SetIndexCount(uint count)
{
	uiIndexCount = count;
}


void RenderData::GenerateBuffers(uchar uiBuffers)
{
	DestroyBuffers(uiBuffers);

	if (uiBuffers & Buffers::VAO)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	if (uiBuffers & Buffers::VBO)
	{
		glGenBuffers(1, &vbo);
		if (uiBuffers & Buffers::VAO)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
		}
	}

	if (uiBuffers & Buffers::IBO)
	{
		glGenBuffers(1, &ibo);
		if (uiBuffers & Buffers::IBO)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}

	if (uiBuffers & Buffers::VAO)
		glBindVertexArray(0);
}


void RenderData::DestroyBuffers(uchar uiBuffers)
{
	if (uiBuffers & Buffers::VAO && vao > 0)
	{
		glDeleteVertexArrays(1, &vao);
	}

	if (uiBuffers & Buffers::VAO && vbo > 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	if (uiBuffers & Buffers::IBO && ibo > 0)
	{
		glDeleteBuffers(1, &ibo);
	}

}


uint RenderData::GetVAO() const
{
	return vao;
}


uint RenderData::GetVBO() const
{
	return vbo;
}


uint RenderData::GetIBO() const
{
	return ibo;
}


void RenderData::CheckBuffers(uchar& uiBuffers)
{
	uiBuffers = 0;

	PipePositive(uiBuffers, vao, Buffers::VAO);
	PipePositive(uiBuffers, vbo, Buffers::VBO);
	PipePositive(uiBuffers, ibo, Buffers::IBO);
}


void RenderData::Bind()
{
	if (vao >  0)
	{
		glBindVertexArray(vao);
	}
	else if (vbo > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		if (ibo > 0)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}
}


void RenderData::Render()
{
	if (ibo)
	{
		glDrawElements(GL_TRIANGLES, uiIndexCount, GL_UNSIGNED_INT, 0);
	}
}


void RenderData::Clear()
{
	vao = 0;
	vbo = 0;
	ibo = 0;
}


void RenderData::Unbind()
{
	if (vao > 0)
	{
		glBindVertexArray(0);
	}
	else if (vbo > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (ibo > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}