#include "LineShader.h"

#include "error.h"

#include <glad/glad.h>
#include <exception>

// initialise private members
LineShader::LineShader() : mMVPUniformLoc(0)
{
}

// creates the shader program on the gpu. returns true if successful
bool LineShader::Create()
{
	try
	{	
		// test for all potential errors when creating program, adding shaders, compiling and setting uniform locations
		if (!Shader::Create())
			throw std::runtime_error("Failed to create LineShader: Shader program failed to create");
		
		if (!AddShader(GL_FRAGMENT_SHADER, "res/Shaders/LineShader/lineshader.frag"))
			throw std::runtime_error("Failed to create LineShader: Fragment shader failed to add");
		
		if (!AddShader(GL_VERTEX_SHADER, "res/Shaders/LineShader/lineshader.vert"))
			throw std::runtime_error("Failed to create LineShader: Vertex shader failed to add");
		
		if (!Compile())
			throw std::runtime_error("Failed to create LineShader: Failed to link");
		
		mMVPUniformLoc = GetUniformLocation("uMVP");
		if (mMVPUniformLoc < 0)
			throw std::runtime_error("Failed to create LineShader: Failed to get uniform location \"uMVP\"");
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return false;
	}

	return true;
}

// set the MVP matrix in the shader program
void LineShader::SetMVPUniform(glm::mat4& pMVPMatrix)
{
	GLCall(glUniformMatrix4fv(mMVPUniformLoc, 1, GL_FALSE, &pMVPMatrix[0][0]));
}
