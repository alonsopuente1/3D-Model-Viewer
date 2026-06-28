#include "Shader.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>

#include "error.h"

Shader::Shader() : mShaderProgram(0)
{
}

bool Shader::Create()
{
	GLCall(mShaderProgram = glCreateProgram());
	return mShaderProgram != 0; // If glCreateProgram returns 0, failed to create program
}

void Shader::Bind()
{
	GLCall(glUseProgram(mShaderProgram));
}

void Shader::Unbind()
{
	GLCall(glUseProgram(0));
}

void Shader::Delete()
{
	glDeleteProgram(mShaderProgram);
}


bool Shader::AddShader(const unsigned int& pShaderType, const char* pFilePath)
{
	GLCall(unsigned int intShader = glCreateShader(pShaderType));
	if (intShader == 0)
	{
		std::cerr << "Failed to add shader" << std::endl;
		return false;
	}
	

	std::string strShaderSource = ReadFile(pFilePath);

	if (strShaderSource == "")
	{
		std::cerr << "Failed to add shader" << std::endl;
		return false;
	}

	const char* cstrShaderSource = strShaderSource.c_str();
	int success = 0;

	GLCall(glShaderSource(intShader, 1, &cstrShaderSource, nullptr));
	GLCall(glCompileShader(intShader));
	GLCall(glGetShaderiv(intShader, GL_COMPILE_STATUS, &success));
	
	if (!success)
	{
		char infoLog[512] = { 0 };
		GLCall(glGetShaderInfoLog(intShader, 512, NULL, infoLog));
		std::cerr << "Failed to compile shader at " << pFilePath << std::endl << "Error: " << infoLog << std::endl;
		return false;
	}
	
	mTempShaderObjects.push_back(intShader);

	return true;
}

bool Shader::Compile()
{
	for (unsigned int intShader : mTempShaderObjects)
	{
		GLCall(glAttachShader(mShaderProgram, intShader));
	}

	GLCall(glLinkProgram(mShaderProgram));

	int success = 0;

	GLCall(glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success));

	if (!success)
	{
		char infoLog[512] = { 0 };
		GLCall(glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog));
		std::cerr << "Failed to link shader program. Error: " << infoLog << std::endl;
		return false;
	}

	return true;
}

int Shader::GetUniformLocation(const char* pUniformName)
{
	GLCall(int intUniformLoc = glGetUniformLocation(mShaderProgram, pUniformName));
	return intUniformLoc;
}

std::string Shader::ReadFile(const char* pFilePath)
{
	try
	{
		std::ifstream file(pFilePath);
		if (!file.is_open())
		{
			std::cerr << "Failed to read file " << pFilePath << std::endl;
			return "";
		}
		std::string strOutput((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		
		return strOutput;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what();
		return "";
	}
}
