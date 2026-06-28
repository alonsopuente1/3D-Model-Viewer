#pragma once

/*
VBO.h

class for creating VBOs (Vertex Buffer Object) which is essential for storing the vertex data
in all the meshes such as position coordinates, texture coordinates, normals etc.
*/

class VBO
{
public:
	// initialises the private member
	VBO();

	// creates the VBO with no data on the GPU
	void Create();
	
	// pushses the given data into the VBO 
	void PushData(const void* pData, const unsigned int& pSizeInBytes);
	
	// sets the attribute of the VBO. used for telling the shader where each component of the vertex is
	void SetAttribute(const unsigned int& pIndex, const unsigned int& pNumOfComponents, const unsigned int& pType, const int& pStride, const void* pOffset);
	
	// disables an attribute at a given index
	void DisableAttribute(const unsigned int& pIndex);

	// binds this vbo to be the currently active one in the GPU
	void Bind();

	// unbinds any vbo
	static void Unbind();

	// deallocates and deletes the VBO from the GPU
	void Delete();

private:
	// stores the OpenGL generated ID for the VBO
	unsigned int mVBO;
};

