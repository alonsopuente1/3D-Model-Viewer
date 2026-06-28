#pragma once

/*
MainShader.h

contains the MainShader class of the program which is used to render the model loaded
using basic shading techniques
*/

#include "Shader.h"

#include <glm/glm.hpp>

class MainShader : public Shader
{

public:
	MainShader();

	bool Create() override;

	void SetMVPUniform(glm::mat4& pMVP);
	void SetModelMatUniform(glm::mat4& pModelMat);
	void SetTextureUnit(int pTexUnit);
	void SetTextureInUse(bool pTextureInUse);
	void SetModelColour(glm::vec3 pModelColour);

private:
	int mMVPUniformLoc;
	int mModelMatUniformLoc;
	int mTextureUnitUniformLoc;
	int mTextureInUseUniformLoc;
	int mModelColourUniformLoc;
};

