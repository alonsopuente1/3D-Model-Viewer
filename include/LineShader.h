#pragma once

/*
LineShader.h

contains the class definition for the LineShader which is used for 
rendering normals and wireframe models
*/

#include "Shader.h"

#include <glm/glm.hpp>

class LineShader : public Shader
{

public:
	LineShader();

	// creates the actual shader
	bool Create() override;

	// sets the Model Projection View matrix uniform inside of the shader program to the value passed
	void SetMVPUniform(glm::mat4& pMVPMatrix);

private:
	// stores the location of the uniform in the shader 
	int mMVPUniformLoc;
};

