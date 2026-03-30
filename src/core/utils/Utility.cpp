#include "Utility.h"
#include <iostream>
#include <string>

// 暂时禁用GLCall宏
#undef GLCall

void Utility::GLCall(const char* function, const char* file, int line)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::string errorString;
        switch (error)
        {
        case GL_INVALID_ENUM:
            errorString = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            errorString = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorString = "GL_INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            errorString = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            errorString = "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            errorString = "GL_OUT_OF_MEMORY";
            break;
        default:
            errorString = "Unknown error";
            break;
        }
        std::cout << "OpenGL Error: " << errorString << " in " << function << " at " << file << ":" << line << std::endl;
    }
}
