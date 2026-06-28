#pragma once

/*
Model.h

header file which contains the decleration of the Model class.
this class was mostly taken from the learnopengl.com website, specifically at https://learnopengl.com/Model-Loading/Model 

*/

#include "Mesh.h"

#include "LineShader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{

public:

	Model();

	// load a model file from specified filepath
	bool LoadModel(const char* pstrFilePath);
	// draw the model using the mainshader
	void Draw(MainShader& pShader);
	// draw the wireframe of model using the lineshader
	void DrawWireframe(LineShader& pShader);
	// draw normals of the model using lineshader
	void DrawNormals(LineShader& pShader);

	// deallocate all memory to the model
	void Delete();

private:
	std::vector<Mesh> mMeshes;

	// funcions taken and altered slightly from https://learnopengl.com/Model-Loading/Model 
	void ProcessNode(aiNode* pNode, const aiScene* pScene);
	Mesh ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
};

