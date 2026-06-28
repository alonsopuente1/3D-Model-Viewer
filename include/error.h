#pragma once

/*
error.h

macro and functions for OpenGL debugging
only works if program is ran in debug mode on visual studio
*/

#include <glad/glad.h>
#include <iostream>

#ifndef NDEBUG

// essentially acts as a breakpoint in visual studio only
#define ASSERT(x) if(!(x)) __debugbreak();

// use this to call any OpenGL function to check for errors
#define GLCall(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// clears the OpenGL error queue
void GLClearError();
// logs the error code, function, file and location of the error
bool GLLogCall(const char* function, const char* file, const int& line);

#else  

#define GLCall(x) x

#endif