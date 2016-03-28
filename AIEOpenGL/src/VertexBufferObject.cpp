#include "VertexBufferObject.h"



VertexBufferObject::VertexBufferObject()
{
}


VertexBufferObject::~VertexBufferObject()
{
}


void VertexBufferObject::Create()
{
	glGenVertexArrays(1, &id);
}


void VertexBufferObject::Use()
{
	glBindVertexArray(id);
}


void VertexBufferObject::Unbind()
{
	glBindVertexArray(0);
}


void VertexBufferObject::GLDelete()
{
	glDeleteVertexArrays(1, &id);
}