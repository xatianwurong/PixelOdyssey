#include "ShaderCompiler.h"
#include "core/logging/Logger.h"
#include <stdexcept>
#include <sstream>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

/**
 * @brief 编译单个着色器
 * @param type 着色器类型（GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER）
 * @param source 着色器源代码
 * @return 着色器对象 ID
 * @details 创建、编译并验证着色器
 */
uint32_t ShaderCompiler::CompileShader(uint32_t type, const std::string& source)
{
    if (!ValidateSource(source))
    {
        LOG_ERROR("ShaderCompiler", "Invalid shader source");
        throw std::invalid_argument("Shader source is empty or invalid");
    }

    // 创建着色器
    uint32_t shader = glCreateShader(type);
    if (shader == 0)
    {
        LOG_ERROR("ShaderCompiler", "Failed to create shader");
        throw std::runtime_error("Failed to create shader");
    }

    // 编译着色器
    const char* src_ptr = source.c_str();
    glShaderSource(shader, 1, &src_ptr, nullptr);
    glCompileShader(shader);

    // 检查编译错误
    int32_t success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        std::string error = GetCompileError(shader);
        LOG_ERROR("ShaderCompiler", "Shader compilation failed: " + error);
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed: " + error);
    }

    std::string type_str = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
    LOG_DEBUG("ShaderCompiler", type_str + " shader compiled successfully");

    return shader;
}

/**
 * @brief 编译着色器程序
 * @param vertexSource 顶点着色器源代码
 * @param fragmentSource 片段着色器源代码
 * @return 着色器程序 ID
 * @details 编译两个着色器并链接成程序
 */
uint32_t ShaderCompiler::CompileProgram(const std::string& vertexSource,
                                       const std::string& fragmentSource)
{
    // 编译着色器
    uint32_t vs = 0, fs = 0;

    try
    {
        vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
        fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    }
    catch (const std::exception&)
    {
        if (vs != 0) glDeleteShader(vs);
        if (fs != 0) glDeleteShader(fs);
        throw;
    }

    // 创建程序
    uint32_t program = glCreateProgram();
    if (program == 0)
    {
        LOG_ERROR("ShaderCompiler", "Failed to create shader program");
        glDeleteShader(vs);
        glDeleteShader(fs);
        throw std::runtime_error("Failed to create shader program");
    }

    // 链接程序
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // 检查链接错误
    int32_t success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        std::string error = GetLinkError(program);
        LOG_ERROR("ShaderCompiler", "Program linking failed: " + error);
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        throw std::runtime_error("Program linking failed: " + error);
    }

    // 删除着色器对象（程序已链接）
    glDeleteShader(vs);
    glDeleteShader(fs);

    LOG_DEBUG("ShaderCompiler", "Shader program linked successfully");

    return program;
}

/**
 * @brief 获取编译错误信息
 * @param shader 着色器对象 ID
 * @return 错误信息字符串
 */
std::string ShaderCompiler::GetCompileError(uint32_t shader)
{
    int32_t maxLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);

    if (maxLen == 0)
    {
        return "Unknown error";
    }

    std::string error(maxLen, '\0');
    glGetShaderInfoLog(shader, maxLen, &maxLen, &error[0]);

    return error;
}

/**
 * @brief 获取链接错误信息
 * @param program 程序对象 ID
 * @return 错误信息字符串
 */
std::string ShaderCompiler::GetLinkError(uint32_t program)
{
    int32_t maxLen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);

    if (maxLen == 0)
    {
        return "Unknown error";
    }

    std::string error(maxLen, '\0');
    glGetProgramInfoLog(program, maxLen, &maxLen, &error[0]);

    return error;
}

/**
 * @brief 验证着色器源代码
 * @param source 源代码字符串
 * @return 如果有效返回 true
 * @details 检查源代码是否包含基本结构
 */
bool ShaderCompiler::ValidateSource(const std::string& source)
{
    if (source.empty())
    {
        return false;
    }

    // 检查是否包含主要着色器结构
    if (source.find("void main()") == std::string::npos)
    {
        LOG_WARNING("ShaderCompiler", "Shader source does not contain main() function");
        // 不抛出异常，某些着色器可能是片段形式
    }

    return true;
}
