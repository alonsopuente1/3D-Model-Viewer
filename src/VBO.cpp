#include "VBO.h"

#include <glad/glad.h>

#include "error.h"

VBO::VBO() : mVBO(0)
{
}

void VBO::Create()
{
	GLCall(glGenBuffers(1, &mVBO));
}

void VBO::PushData(const void* pData, const unsigned int& pSizeInBytes)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, pSizeInBytes, pData, GL_STATIC_DRAW));
	Unbind();
}

void VBO::SetAttribute(const unsigned int& pIndex, const unsigned int& pNumOfComponents, const unsigned int& pType, const int& pStride, const void* pOffset)
{
	GLCall(glVertexAttribPointer(pIndex, pNumOfComponents, pType, GL_FALSE, pStride, pOffset));
	GLCall(glEnableVertexAttribArray(pIndex));
}

void VBO::DisableAttribute(const unsigned int& pIndex)
{
	GLCall(glDisableVertexAttribArray(pIndex));
}

void VBO::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
}

void VBO::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::Delete()
{
	glDeleteBuffers(1, &mVBO);
}
