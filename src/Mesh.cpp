#include "Mesh.h"

#include <glad/glad.h>
#include <Windows.h>

#include "error.h"

// initialise private members
Mesh::Mesh() : mNumIndices(0), mNumNormals(0)
{
}

// create the mesh and fill the buffers with the given indices and vertices
void Mesh::CreateMesh(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices)
{
	if (pVertices.size() <= 0 || pIndices.size() <= 0)
	{
		std::cerr << "No vertices or no indices passed to create mesh!" << std::endl;
		return;
	}
	// the program can only handle rendering triangles, so if the indices passed through isn't divisible by 3 then stop
	else if (pIndices.size() % 3 != 0)
	{
		std::cerr << "Invalid pIndices parameter! Must be perfectly divisible by 3 to create mesh" << std::endl;
		return;
	}

	// delete all existing data
	mMeshVAO.Delete();
	mMeshVBO.Delete();
	mMeshEBO.Delete();

	mNormalVAO.Delete();
	mNormalVBO.Delete();

	// create the new OpenGL objects
	mMeshVAO.Create();
	mMeshVBO.Create();
	mMeshEBO.Create();

	// push the vertex and index data
	mMeshVBO.PushData(&pVertices[0], sizeof(Vertex) * pVertices.size());
	mMeshEBO.PushData(&pIndices[0], sizeof(unsigned int) * pIndices.size());

	// bind the vertex array object to save which objects to bind for the mesh
	mMeshVAO.Bind();

	// bind vertex buffer object and set its attributes
	mMeshVBO.Bind();
	mMeshVBO.SetAttribute(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	mMeshVBO.SetAttribute(1, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(Vertex::pos)));
	mMeshVBO.SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(Vertex::pos) + sizeof(Vertex::normal)));
	// bind element buffer object (indices)
	mMeshEBO.Bind();

	// unbind the VAO to save the state of which buffer objects to bind
	mMeshVAO.Unbind();

	// unbind the other buffer objects
	mMeshVBO.Unbind();
	mMeshEBO.Unbind();

	// save the number of indices in the mesh for the OpenGL draw call
	mNumIndices = pIndices.size();
	
	CreateNormals(pVertices, pIndices);
}

// overloaded create mesh function but with a texture as well
void Mesh::CreateMesh(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices, const char* pTexturePath)
{
	// create mesh as normal
	CreateMesh(pVertices, pIndices);
	
	// then attempt to load texture, display error message on failure
	if (!mTexture.LoadTexture(pTexturePath))
	{
		std::string strErrMsg = "Missing texture file at path '" + std::string(pTexturePath) + "'!";
		MessageBoxA(NULL, strErrMsg.c_str(), NULL, MB_OK);
	}
}

// draws the mesh onto the screen
void Mesh::Draw(MainShader& pShader)
{
	pShader.Bind();

	// if there is a texture then set the necessary uniforms in the shader and vice versa
	if (mTexture.IsCreated())
	{
		pShader.SetTextureInUse(true);
		pShader.SetTextureUnit(0);
		mTexture.Bind(0);
	}
	else
	{
		pShader.SetTextureInUse(false);
	}

	// perform the OpenGL draw call
	mMeshVAO.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, (void*)0));
	mMeshVAO.Unbind();
}

// draws the normals of the mesh
void Mesh::DrawNormals(LineShader& pShader)
{
	pShader.Bind();
	mNormalVAO.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, mNumNormals));
	mNormalVAO.Unbind();
}

// draws the wireframe of the mesh
void Mesh::DrawWireframe(LineShader& pShader)
{
	pShader.Bind();
	mMeshVAO.Bind();
	GLCall(glDrawElements(GL_LINES, mNumIndices, GL_UNSIGNED_INT, (void*)0));
	mMeshVAO.Unbind();
}

// deletes all OpenGL objects belonging to the mesh
void Mesh::Delete()
{
	mMeshVAO.Delete();
	mMeshVBO.Delete();
	mMeshEBO.Delete();
	mNormalVAO.Delete();
	mNormalVBO.Delete();
	mTexture.Delete();
}

// creates the world space normals given the vertices and indices
void Mesh::CreateNormals(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices)
{
	// initialise an empty array
	std::vector<glm::vec3> arrNormals = {};

	try
	{
		// for each 3 indices try and generate a normal, then push it to empty array
		for (int i = 0; i < pIndices.size(); i += 3)
		{
			Normal tmpNormal = GenerateNormalFromTri(pVertices.at(pIndices.at(i)).pos, pVertices.at(pIndices.at(i + 1u)).pos, pVertices.at(pIndices.at(i + 2u)).pos);

			arrNormals.push_back(tmpNormal.normal[0]);
			arrNormals.push_back(tmpNormal.normal[1]);
		}
	}
	// if exception occured print error and return
	catch (const std::exception& ex)
	{
		std::cerr << "Failed to create normals for mesh: " << ex.what() << std::endl;
		return;
	}
	
	// delete existing data (if any)
	mNormalVAO.Delete();
	mNormalVBO.Delete();

	// intialise buffer objects
	mNormalVAO.Create();
	mNormalVBO.Create();

	// push the normals to the buffer object
	mNormalVBO.PushData(&arrNormals[0], sizeof(glm::vec3) * arrNormals.size());

	// bind vertex array object, save the buffer objects bindings and attibute then unbind vertex array object
	mNormalVAO.Bind();
	mNormalVBO.Bind();
	mNormalVBO.SetAttribute(0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	mNormalVAO.Unbind();

	
	mNormalVBO.Unbind();

	// used for the draw call
	mNumNormals = arrNormals.size();
}

Mesh::Normal Mesh::GenerateNormalFromTri(glm::vec3 pPos1, glm::vec3 pPos2, glm::vec3 pPos3)
{
	Normal nmlOutput;

	// sets first element to middle of the triangle
	nmlOutput.normal[0] = (pPos1 + pPos2 + pPos3) / 3.f;

	// gets the normal to the triangle
	glm::vec3 normal = glm::cross(pPos2 - pPos1, pPos3 - pPos1);
	glm::normalize(normal);

	// sets the second element to the middle plus the normal
	nmlOutput.normal[1] = nmlOutput.normal[0] + normal;

	return nmlOutput;
}

