#include "VAO.h"

#include <glad/glad.h>

#include "error.h"

VAO::VAO() : mVAO(0)
{
}

void VAO::Create()
{
	GLCall(glGenVertexArrays(1, &mVAO));
}

void VAO::Bind()
{
	GLCall(glBindVertexArray(mVAO));
}

void VAO::Unbind()
{
	GLCall(glBindVertexArray(0));
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &mVAO);
}
