#include "ShaderGL.h"
#include "core/logging/Logger.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

/**
 * @brief 构造函数
 * @param vertexSource 顶点着色器源代码
 * @param fragmentSource 片段着色器源代码
 * @details 编译并链接着色器程序
 */
ShaderGL::ShaderGL(const std::string& vertexSource, const std::string& fragmentSource)
{
    try
    {
        m_program = ShaderCompiler::CompileProgram(vertexSource, fragmentSource);
        LOG_DEBUG("ShaderGL", "Created shader program (ID=" + std::to_string(m_program) + ")");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("ShaderGL", "Failed to create shader: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief 析构函数
 * @details 清理着色器程序资源
 */
ShaderGL::~ShaderGL()
{
    DestroyProgram();
}

/**
 * @brief 绑定着色器
 * @details 使用着色器程序
 */
void ShaderGL::Bind() const
{
    if (!IsValid())
    {
        LOG_WARNING("ShaderGL", "Attempting to bind invalid shader");
        return;
    }
    glUseProgram(m_program);
}

/**
 * @brief 解绑着色器
 * @details 停止使用当前着色器程序
 */
void ShaderGL::Unbind() const
{
    glUseProgram(0);
}

/**
 * @brief 设置整数 uniform 变量
 * @param name uniform 变量名
 * @param value 整数值
 */
void ShaderGL::SetUniform(const char* name, int value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniform1i(loc, value);
    }
}

/**
 * @brief 设置浮点数 uniform 变量
 * @param name uniform 变量名
 * @param value 浮点数值
 */
void ShaderGL::SetUniform(const char* name, float value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniform1f(loc, value);
    }
}

/**
 * @brief 设置 vec2 uniform 变量
 * @param name uniform 变量名
 * @param value vec2 值
 */
void ShaderGL::SetUniform(const char* name, const glm::vec2& value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniform2fv(loc, 1, glm::value_ptr(value));
    }
}

/**
 * @brief 设置 vec3 uniform 变量
 * @param name uniform 变量名
 * @param value vec3 值
 */
void ShaderGL::SetUniform(const char* name, const glm::vec3& value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }
}

/**
 * @brief 设置 vec4 uniform 变量
 * @param name uniform 变量名
 * @param value vec4 值
 */
void ShaderGL::SetUniform(const char* name, const glm::vec4& value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniform4fv(loc, 1, glm::value_ptr(value));
    }
}

/**
 * @brief 设置 mat4 uniform 变量
 * @param name uniform 变量名
 * @param value 4x4 矩阵值
 */
void ShaderGL::SetUniform(const char* name, const glm::mat4& value)
{
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, 0, glm::value_ptr(value));
    }
}

/**
 * @brief 获取 uniform 变量位置
 * @param name uniform 变量名
 * @return uniform 位置，未找到返回 -1
 */
int ShaderGL::GetUniformLocation(const char* name) const
{
    return GetCachedUniformLocation(name);
}

/**
 * @brief 获取缓存的 uniform 变量位置
 * @param name uniform 变量名
 * @return uniform 位置，未找到返回 -1
 * @details 使用缓存避免重复查询 OpenGL
 */
int32_t ShaderGL::GetCachedUniformLocation(const char* name) const
{
    if (!IsValid() || !name)
    {
        return -1;
    }

    // 检查缓存
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end())
    {
        return it->second;
    }

    // 从 OpenGL 获取位置
    int32_t loc = glGetUniformLocation(m_program, name);

    if (loc == -1)
    {
        LOG_WARNING("ShaderGL", std::string("Uniform not found: ") + name);
    }
    else
    {
        // 缓存位置
        m_uniformCache[name] = loc;
    }

    return loc;
}

/**
 * @brief 销毁着色器程序
 * @details 删除 OpenGL 程序对象并清空缓存
 */
void ShaderGL::DestroyProgram()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
        m_uniformCache.clear();
        LOG_DEBUG("ShaderGL", "Deleted shader program");
    }
}
