#include "error.h"

#ifndef NDEBUG

// clears all OpenGL errors in the queue
void GLClearError()
{
    int i = 0;
    while (glGetError() != GL_NO_ERROR || ++i <= 500);
}

// prints every error in the queue and shows function name location of where it occured
bool GLLogCall(const char* function, const char* file, const int& line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "OpenGL Error (" << std::hex << error << "): Function " << function << " in file " << file << " on line " << line << std::endl;
        return false;
    }
    return true;
}

#endif