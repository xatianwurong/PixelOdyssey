#include "ShaderGL.h"
#include "core/logging/Logger.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

ShaderGL::ShaderGL(const std::string& vertexSource, const std::string& fragmentSource) {
    try {
        m_program = ShaderCompiler::CompileProgram(vertexSource, fragmentSource);
        LOG_DEBUG("ShaderGL", "Created shader program (ID=" + std::to_string(m_program) + ")");
    }
    catch (const std::exception& e) {
        LOG_ERROR("ShaderGL", "Failed to create shader: " + std::string(e.what()));
        throw;
    }
}

ShaderGL::~ShaderGL() {
    DestroyProgram();
}

void ShaderGL::Bind() const {
    if (!IsValid()) {
        LOG_WARNING("ShaderGL", "Attempting to bind invalid shader");
        return;
    }
    glUseProgram(m_program);
}

void ShaderGL::Unbind() const {
    glUseProgram(0);
}

void ShaderGL::SetUniform(const char* name, int value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniform1i(loc, value);
    }
}

void ShaderGL::SetUniform(const char* name, float value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void ShaderGL::SetUniform(const char* name, const glm::vec2& value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniform2fv(loc, 1, glm::value_ptr(value));
    }
}

void ShaderGL::SetUniform(const char* name, const glm::vec3& value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }
}

void ShaderGL::SetUniform(const char* name, const glm::vec4& value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniform4fv(loc, 1, glm::value_ptr(value));
    }
}

void ShaderGL::SetUniform(const char* name, const glm::mat4& value) {
    int32_t loc = GetCachedUniformLocation(name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, 0, glm::value_ptr(value));
    }
}

int ShaderGL::GetUniformLocation(const char* name) const {
    return GetCachedUniformLocation(name);
}

int32_t ShaderGL::GetCachedUniformLocation(const char* name) const {
    if (!IsValid() || !name) {
        return -1;
    }
    
    // 检查缓存
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }
    
    // 从 OpenGL 获取位置
    int32_t loc = glGetUniformLocation(m_program, name);
    
    if (loc == -1) {
        LOG_WARNING("ShaderGL", std::string("Uniform not found: ") + name);
    }
    else {
        // 缓存位置
        m_uniformCache[name] = loc;
    }
    
    return loc;
}

void ShaderGL::DestroyProgram() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
        m_uniformCache.clear();
        LOG_DEBUG("ShaderGL", "Deleted shader program");
    }
}
