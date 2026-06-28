#pragma once

/*
VAO.h

class for creating VAOs (Vertex Array Objects) an OpenGL object necessary for rendering
meshes
*/

class VAO
{
public:
	// initialises the private member
	VAO();

	// create the VAO with no data 
	void Create();

	// bind this VAO
	void Bind();

	// unbind any VAO that might be bound
	static void Unbind();
	
	// delete this VAO
	void Delete();

private:
	// stores the OpenGL generated ID for the VAO
	unsigned int mVAO;
};

