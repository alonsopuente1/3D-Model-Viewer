#pragma once

/*
Mesh.h

class that creates meshes, textured and untextured.
*/

#include "Vertex.h"

#include "MainShader.h"
#include "LineShader.h"

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

#include "Texture.h"

#include <vector>

class Mesh
{

public:
	Mesh();

	void CreateMesh(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices);
	void CreateMesh(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices, const char* pTexturePath);
	void Draw(MainShader& pShader);
	void DrawNormals(LineShader& pShader);
	void DrawWireframe(LineShader& pShader);

	void Delete();

private:

	VAO mMeshVAO;
	VBO mMeshVBO;
	EBO mMeshEBO;

	int mNumIndices;
	int mNumNormals;

	VAO mNormalVAO;
	VBO mNormalVBO;

	Texture mTexture;

	void CreateNormals(std::vector<Vertex>& pVertices, std::vector<unsigned int>& pIndices);

	struct Normal {
		glm::vec3 normal[2];
	};

	Normal GenerateNormalFromTri(glm::vec3 pPos1, glm::vec3 pPos2, glm::vec3 pPos3);
};

