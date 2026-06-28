#pragma once

#include <vector>
#include <iostream>

class Shader
{
public:
	Shader();

	virtual bool Create();
	void Bind();
	static void Unbind();
	void Delete();

protected:
	bool AddShader(const unsigned int& pShaderType, const char* pFilePath);
	bool Compile();
	int GetUniformLocation(const char* pUniformName);
	std::string ReadFile(const char* pFilePath);
private:
	unsigned int mShaderProgram;
	std::vector<unsigned int> mTempShaderObjects;

};

