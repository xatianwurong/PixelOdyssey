#pragma once
#include <GL/GLEW.h>

/**
 * @class Utility
 * @brief 工具类
 * @details 提供通用的工具函数，如OpenGL错误检查等
 * @coreResponsibility 提供辅助功能，简化代码
 * @designIntent 封装常用的工具函数，提高代码复用性
 * @useCase 用于OpenGL错误检查、日志记录等
 * @attention 提供静态方法，无需实例化
 */
class Utility
{
public:
    static void GLCall(const char* function, const char* file, int line);
};

// 宏定义，用于自动传递函数名、文件名和行号
#define GLCall(x) Utility::GLCall(#x, __FILE__, __LINE__); x
