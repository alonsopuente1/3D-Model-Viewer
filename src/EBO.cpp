#include "EBO.h"

#include <glad/glad.h>

#include "error.h"

// initialise the private members
EBO::EBO() : mEBO(0)
{
}

// create identifier for buffer object
void EBO::Create()
{
	GLCall(glGenBuffers(1, &mEBO));
}

// push the data at pData into the buffer
void EBO::PushData(const void* pData, const unsigned int& pSize)
{
	Bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, pSize, pData, GL_STATIC_DRAW));
	Unbind();
}

// deletes all data in the buffer object
void EBO::Delete()
{
	glDeleteBuffers(1, &mEBO);
}

// binds this buffer to the element buffer array
void EBO::Bind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO));
}

// ubinds whatever is in the element buffer array
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
