#pragma once

/*
EBO.h

defines the class for creating EBOs (Element buffer object) in order
to render meshes
*/

class EBO
{

public:
	// initialises the private member
	EBO();

	// creates the EBO with no data on the GPU
	void Create();
	// pushes all the data passed to this EBO
	void PushData(const void* pData, const unsigned int& pSize);
	// deallocates data pushed to this EBO
	void Delete();
	// binds this EBO
	void Bind();

	// unbinds any EBO currently bound
	static void Unbind();
private:
	// stores the OpenGL generated ID for the EBO
	unsigned int mEBO;
};

