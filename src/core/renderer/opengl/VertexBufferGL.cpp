#include "VertexBufferGL.h"
#include "core/logging/Logger.h"
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

/**
 * @brief 构造函数
 * @param data 顶点数据指针
 * @param size 数据大小（字节）
 * @param usage 缓冲区使用模式
 * @details 创建 OpenGL VBO 和 VAO 并上传顶点数据
 */
VertexBufferGL::VertexBufferGL(const void* data, uint32_t size, BufferUsage usage)
    : m_size(size), m_usage(usage)
{
    if (!data || size == 0)
    {
        LOG_ERROR("VertexBufferGL", "Invalid data or size");
        throw std::invalid_argument("VertexBuffer data cannot be null or empty");
    }

    try
    {
        CreateBuffers(data, size);
        LOG_DEBUG("VertexBufferGL", "Created VBO (size=" + std::to_string(size) + " bytes)");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("VertexBufferGL", "Failed to create buffers: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief 析构函数
 * @details 清理 OpenGL VBO 和 VAO 资源
 */
VertexBufferGL::~VertexBufferGL()
{
    DestroyBuffers();
}

/**
 * @brief 获取缓冲区类型
 * @return BufferType::Vertex
 */
BufferType VertexBufferGL::GetType() const
{
    return BufferType::Vertex;
}

/**
 * @brief 获取缓冲区大小
 * @return 缓冲区大小（字节）
 */
uint32_t VertexBufferGL::GetSize() const
{
    return m_size;
}

/**
 * @brief 获取缓冲区使用模式
 * @return BufferUsage 枚举值
 */
BufferUsage VertexBufferGL::GetUsage() const
{
    return m_usage;
}

/**
 * @brief 绑定缓冲区
 * @details 将 VAO 和 VBO 绑定到 OpenGL 上下文
 */
void VertexBufferGL::Bind() const
{
    if (!IsValid())
    {
        LOG_WARNING("VertexBufferGL", "Attempting to bind invalid buffer");
        return;
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

/**
 * @brief 解绑缓冲区
 * @details 从 OpenGL 上下文解绑 VBO 和 VAO
 */
void VertexBufferGL::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief 更新缓冲区数据
 * @param data 新数据指针
 * @param size 数据大小（字节）
 * @details 更新缓冲区中的部分或全部数据
 * @details 仅支持 Dynamic 和 Stream 模式的缓冲区
 */
void VertexBufferGL::SetData(const void* data, uint32_t size)
{
    if (!data || size == 0)
    {
        LOG_ERROR("VertexBufferGL", "Invalid data for SetData");
        return;
    }

    if (size > m_size)
    {
        LOG_WARNING("VertexBufferGL", "New size exceeds original size");
        return;
    }

    if (m_usage == BufferUsage::Static)
    {
        LOG_WARNING("VertexBufferGL", "Cannot update static buffer");
        return;
    }

    try
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        Unbind();
        LOG_DEBUG("VertexBufferGL", "Updated buffer data (" + std::to_string(size) + " bytes)");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("VertexBufferGL", "Failed to update buffer: " + std::string(e.what()));
    }
}

/**
 * @brief 创建缓冲区
 * @param data 顶点数据指针
 * @param size 数据大小（字节）
 * @details 创建 OpenGL VAO 和 VBO 并上传顶点数据
 */
void VertexBufferGL::CreateBuffers(const void* data, uint32_t size)
{
    // 创建 VAO
    glGenVertexArrays(1, &m_vao);
    if (m_vao == 0)
    {
        throw std::runtime_error("Failed to create Vertex Array Object");
    }

    // 绑定 VAO
    glBindVertexArray(m_vao);

    // 创建 VBO
    glGenBuffers(1, &m_vbo);
    if (m_vbo == 0)
    {
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

/**
 * @brief 获取 OpenGL 使用模式提示
 * @return OpenGL 缓冲区使用模式常量
 * @details 根据 BufferUsage 返回对应的 OpenGL 常量
 */
uint32_t VertexBufferGL::GetGLUsageHint() const
{
    switch (m_usage)
    {
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

/**
 * @brief 销毁缓冲区
 * @details 删除 OpenGL VBO 和 VAO 资源
 */
void VertexBufferGL::DestroyBuffers()
{
    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
        LOG_DEBUG("VertexBufferGL", "Deleted VBO");
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
        LOG_DEBUG("VertexBufferGL", "Deleted VAO");
    }
}
