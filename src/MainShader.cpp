#include "MainShader.h"

#include <glad/glad.h>

#include "error.h"

// initialise private members
MainShader::MainShader() 
	: mMVPUniformLoc(0), 
	mModelMatUniformLoc(0),
	mTextureInUseUniformLoc(0),
	mTextureUnitUniformLoc(0),
	mModelColourUniformLoc(0)
{
}

// creates the shader program on the gpu. returns true if successful
bool MainShader::Create()
{
	try
	{
		// test for all potential errors when creating program, adding shaders, compiling and setting uniform locations
		if (!Shader::Create())
			throw std::runtime_error("Failed to create MainShader: Shader program failed to create");

		if (!AddShader(GL_VERTEX_SHADER, "res/Shaders/MainShader/mainshader.vert"))
			throw std::runtime_error("Failed to create MainShader: Vertex shader failed to add");

		if (!AddShader(GL_FRAGMENT_SHADER, "res/Shaders/MainShader/mainshader.frag"))
			throw std::runtime_error("Failed to create MainShader: Fragment shader failed to add");

		if(!Compile())
			throw std::runtime_error("Failed to create MainShader: Failed to link");

		mMVPUniformLoc = GetUniformLocation("uMVP");
		if (mMVPUniformLoc < 0)
			throw std::runtime_error("Failed to create MainShader: Failed to get uniform location \"uMVP\"");

		mModelMatUniformLoc = GetUniformLocation("uModelMat");
		if (mModelMatUniformLoc < 0)
			throw std::runtime_error("Failed to create MainShader: Failed to get uniform location \"uModelMat\""); 

		mTextureUnitUniformLoc = GetUniformLocation("uTexture");
		if (mTextureUnitUniformLoc < 0)
			throw std::runtime_error("Failed to create MainShader: Failed to get uniform location \"uTexture\"");

		mTextureInUseUniformLoc = GetUniformLocation("uUseTexture");
		if (mTextureInUseUniformLoc < 0)
			throw std::runtime_error("Failed to create MainShader: Failed to get uniform location \"uUseTexture\"");

		mModelColourUniformLoc = GetUniformLocation("uObjectColour");
		if (mModelColourUniformLoc < 0)
			throw std::runtime_error("Failed to create MainShader: Failed to get uniform location \"uObjectColour\"");
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return false;
	}

	return true;
}

// set the model, view and projection matrix of the shader program
void MainShader::SetMVPUniform(glm::mat4& pMVP)
{
	GLCall(glUniformMatrix4fv(mMVPUniformLoc, 1, GL_FALSE, &pMVP[0][0]));
}

// set the model matrix of the shader program
void MainShader::SetModelMatUniform(glm::mat4& pModelMat)
{
	GLCall(glUniformMatrix4fv(mModelMatUniformLoc, 1, GL_FALSE, &pModelMat[0][0]));
}

// set which texture unit to use in the shader
void MainShader::SetTextureUnit(int pTexUnit)
{
	GLCall(glUniform1i(mTextureUnitUniformLoc, pTexUnit));
}

// tell the shader whether a texture is being used or not
void MainShader::SetTextureInUse(bool pTextureInUse)
{
	GLCall(glUniform1i(mTextureInUseUniformLoc, (int)pTextureInUse));
}

// sets the colour of the model
void MainShader::SetModelColour(glm::vec3 pModelColour)
{
	GLCall(glUniform3f(mModelColourUniformLoc, pModelColour.r, pModelColour.g, pModelColour.b));
}
