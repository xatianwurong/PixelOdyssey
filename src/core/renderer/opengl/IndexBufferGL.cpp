#include "IndexBufferGL.h"
#include "core/logging/Logger.h"
#include <stdexcept>

// Include GLEW for OpenGL function loading
#include <GL/glew.h>

using namespace Logging;

/**
 * @brief 构造函数
 * @param indices 索引数组指针
 * @param count 索引数量
 * @details 创建 OpenGL 索引缓冲区并上传数据
 */
IndexBufferGL::IndexBufferGL(const uint32_t* indices, uint32_t count)
    : m_count(count)
{
    if (!indices || count == 0)
    {
        LOG_ERROR("IndexBufferGL", "Invalid indices or count");
        throw std::invalid_argument("IndexBuffer indices cannot be null or empty");
    }

    try
    {
        CreateBuffer(indices, count);
        LOG_DEBUG("IndexBufferGL", "Created IBO (count=" + std::to_string(count) + ")");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("IndexBufferGL", "Failed to create buffer: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief 析构函数
 * @details 清理 OpenGL 缓冲区资源
 */
IndexBufferGL::~IndexBufferGL()
{
    DestroyBuffer();
}

/**
 * @brief 获取缓冲区类型
 * @return BufferType::Index
 */
BufferType IndexBufferGL::GetType() const
{
    return BufferType::Index;
}

/**
 * @brief 获取缓冲区大小
 * @return 缓冲区大小（字节）
 */
uint32_t IndexBufferGL::GetSize() const
{
    return m_count * sizeof(uint32_t);
}

/**
 * @brief 获取缓冲区使用模式
 * @return BufferUsage::Static
 */
BufferUsage IndexBufferGL::GetUsage() const
{
    return BufferUsage::Static;
}

/**
 * @brief 绑定缓冲区
 * @details 将缓冲区绑定到 OpenGL 上下文
 */
void IndexBufferGL::Bind() const
{
    if (!IsValid())
    {
        LOG_WARNING("IndexBufferGL", "Attempting to bind invalid buffer");
        return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

/**
 * @brief 解绑缓冲区
 * @details 从 OpenGL 上下文解绑缓冲区
 */
void IndexBufferGL::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * @brief 更新缓冲区数据
 * @param data 新数据指针
 * @param size 数据大小（字节）
 * @details 更新缓冲区中的部分或全部数据
 */
void IndexBufferGL::SetData(const void* data, uint32_t size)
{
    if (!data || size == 0)
    {
        LOG_ERROR("IndexBufferGL", "Invalid data for SetData");
        return;
    }

    if (size > GetSize())
    {
        LOG_WARNING("IndexBufferGL", "New size exceeds original size");
        return;
    }

    try
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
        Unbind();
        LOG_DEBUG("IndexBufferGL", "Updated buffer data");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("IndexBufferGL", "Failed to update buffer: " + std::string(e.what()));
    }
}

/**
 * @brief 创建缓冲区
 * @param indices 索引数组指针
 * @param count 索引数量
 * @details 创建 OpenGL EBO 并上传索引数据
 */
void IndexBufferGL::CreateBuffer(const uint32_t* indices, uint32_t count)
{
    // 创建 EBO
    glGenBuffers(1, &m_ebo);
    if (m_ebo == 0)
    {
        throw std::runtime_error("Failed to create Element Buffer Object");
    }

    // 绑定和上传数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    LOG_DEBUG("IndexBufferGL", "EBO=" + std::to_string(m_ebo));
}

/**
 * @brief 销毁缓冲区
 * @details 删除 OpenGL EBO 资源
 */
void IndexBufferGL::DestroyBuffer()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
        LOG_DEBUG("IndexBufferGL", "Deleted EBO");
    }
}
