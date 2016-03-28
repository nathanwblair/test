#include "VertexArrayObject.h"
#include "Utils.h"


VertexArrayObject::VertexArrayObject()
{
}


VertexArrayObject::~VertexArrayObject()
{
}


void VertexArrayObject::Create()
{
	glGenVertexArrays(1, &id);
}


void VertexArrayObject::Use()
{
	glBindVertexArray(id);
}


void VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}


void VertexArrayObject::GLDelete()
{
	glDeleteVertexArrays(1, &id);
}