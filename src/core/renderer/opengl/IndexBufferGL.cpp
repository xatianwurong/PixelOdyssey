#include "IndexBufferGL.h"
#include "core/logging/Logger.h"
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

IndexBufferGL::IndexBufferGL(const uint32_t* indices, uint32_t count) 
    : m_count(count) {
    
    if (!indices || count == 0) {
        LOG_ERROR("IndexBufferGL", "Invalid indices or count");
        throw std::invalid_argument("IndexBuffer indices cannot be null or empty");
    }
    
    try {
        CreateBuffer(indices, count);
        LOG_DEBUG("IndexBufferGL", "Created IBO (count=" + std::to_string(count) + ")");
    }
    catch (const std::exception& e) {
        LOG_ERROR("IndexBufferGL", "Failed to create buffer: " + std::string(e.what()));
        throw;
    }
}

IndexBufferGL::~IndexBufferGL() {
    DestroyBuffer();
}

BufferType IndexBufferGL::GetType() const {
    return BufferType::Index;
}

uint32_t IndexBufferGL::GetSize() const {
    return m_count * sizeof(uint32_t);
}

BufferUsage IndexBufferGL::GetUsage() const {
    return BufferUsage::Static;
}

void IndexBufferGL::Bind() const {
    if (!IsValid()) {
        LOG_WARNING("IndexBufferGL", "Attempting to bind invalid buffer");
        return;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void IndexBufferGL::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferGL::SetData(const void* data, uint32_t size) {
    if (!data || size == 0) {
        LOG_ERROR("IndexBufferGL", "Invalid data for SetData");
        return;
    }
    
    if (size > GetSize()) {
        LOG_WARNING("IndexBufferGL", "New size exceeds original size");
        return;
    }
    
    try {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
        Unbind();
        LOG_DEBUG("IndexBufferGL", "Updated buffer data");
    }
    catch (const std::exception& e) {
        LOG_ERROR("IndexBufferGL", "Failed to update buffer: " + std::string(e.what()));
    }
}

void IndexBufferGL::CreateBuffer(const uint32_t* indices, uint32_t count) {
    // 创建 EBO
    glGenBuffers(1, &m_ebo);
    if (m_ebo == 0) {
        throw std::runtime_error("Failed to create Element Buffer Object");
    }
    
    // 绑定和上传数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    LOG_DEBUG("IndexBufferGL", "EBO=" + std::to_string(m_ebo));
}

void IndexBufferGL::DestroyBuffer() {
    if (m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
        LOG_DEBUG("IndexBufferGL", "Deleted EBO");
    }
}
