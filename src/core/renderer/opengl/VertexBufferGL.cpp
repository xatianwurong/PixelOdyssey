#include "VertexBufferGL.h"
#include "core/logging/Logger.h"
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

VertexBufferGL::VertexBufferGL(const void* data, uint32_t size, BufferUsage usage)
    : m_size(size), m_usage(usage) {
    
    if (!data || size == 0) {
        LOG_ERROR("VertexBufferGL", "Invalid data or size");
        throw std::invalid_argument("VertexBuffer data cannot be null or empty");
    }
    
    try {
        CreateBuffers(data, size);
        LOG_DEBUG("VertexBufferGL", "Created VBO (size=" + std::to_string(size) + " bytes)");
    }
    catch (const std::exception& e) {
        LOG_ERROR("VertexBufferGL", "Failed to create buffers: " + std::string(e.what()));
        throw;
    }
}

VertexBufferGL::~VertexBufferGL() {
    DestroyBuffers();
}

BufferType VertexBufferGL::GetType() const {
    return BufferType::Vertex;
}

uint32_t VertexBufferGL::GetSize() const {
    return m_size;
}

BufferUsage VertexBufferGL::GetUsage() const {
    return m_usage;
}

void VertexBufferGL::Bind() const {
    if (!IsValid()) {
        LOG_WARNING("VertexBufferGL", "Attempting to bind invalid buffer");
        return;
    }
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void VertexBufferGL::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexBufferGL::SetData(const void* data, uint32_t size) {
    if (!data || size == 0) {
        LOG_ERROR("VertexBufferGL", "Invalid data for SetData");
        return;
    }
    
    if (size > m_size) {
        LOG_WARNING("VertexBufferGL", "New size exceeds original size");
        return;
    }
    
    if (m_usage == BufferUsage::Static) {
        LOG_WARNING("VertexBufferGL", "Cannot update static buffer");
        return;
    }
    
    try {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        Unbind();
        LOG_DEBUG("VertexBufferGL", "Updated buffer data (" + std::to_string(size) + " bytes)");
    }
    catch (const std::exception& e) {
        LOG_ERROR("VertexBufferGL", "Failed to update buffer: " + std::string(e.what()));
    }
}

void VertexBufferGL::CreateBuffers(const void* data, uint32_t size) {
    // 创建 VAO
    glGenVertexArrays(1, &m_vao);
    if (m_vao == 0) {
        throw std::runtime_error("Failed to create Vertex Array Object");
    }
    
    // 绑定 VAO
    glBindVertexArray(m_vao);
    
    // 创建 VBO
    glGenBuffers(1, &m_vbo);
    if (m_vbo == 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
        throw std::runtime_error("Failed to create Vertex Buffer Object");
    }
    
    // 绑定和上传数据
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GetGLUsageHint());
    
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    LOG_DEBUG("VertexBufferGL", "VAO=" + std::to_string(m_vao) + 
              ", VBO=" + std::to_string(m_vbo));
}

uint32_t VertexBufferGL::GetGLUsageHint() const {
    switch (m_usage) {
        case BufferUsage::Static:
            return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
        case BufferUsage::Stream:
            return GL_STREAM_DRAW;
        default:
            return GL_STATIC_DRAW;
    }
}

void VertexBufferGL::DestroyBuffers() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
        LOG_DEBUG("VertexBufferGL", "Deleted VBO");
    }
    
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
        LOG_DEBUG("VertexBufferGL", "Deleted VAO");
    }
}
