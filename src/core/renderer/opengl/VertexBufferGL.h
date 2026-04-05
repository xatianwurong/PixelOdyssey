#pragma once

#include "core/renderer/interfaces/IBuffer.h"
#include <cstdint>

/**
 * @class VertexBufferGL
 * @brief OpenGL 顶点缓冲区实现
 * @details
 *   - 管理 OpenGL Vertex Buffer Object (VBO)
 *   - 支持顶点数据的上传和更新
 *   - 自动管理 Vertex Array Object (VAO)
 *   - 支持三种缓冲区使用模式
 * 
 * @usage
 *   std::vector<float> vertices = { ... };
 *   VertexBufferGL vb(vertices.data(), vertices.size() * sizeof(float), 
 *                     BufferUsage::Static);
 *   vb.Bind();
 *   // 使用顶点数据进行绘制
 *   vb.Unbind();
 * 
 * @thread_safety 不是线程安全的，应在 OpenGL 上下文线程中使用
 */
class VertexBufferGL : public IBuffer {
public:
    /**
     * @brief 构造函数：创建顶点缓冲区
     * @param data 顶点数据指针
     * @param size 数据大小（字节）
     * @param usage 缓冲区使用模式
     * @throw std::runtime_error 如果缓冲区创建失败
     */
    VertexBufferGL(const void* data, uint32_t size, BufferUsage usage);
    
    /**
     * @brief 析构函数：释放 GPU 资源
     */
    ~VertexBufferGL();
    
    // ========== IBuffer 接口实现 ==========
    
    /**
     * @brief 获取缓冲区类型
     * @return BufferType::Vertex
     */
    BufferType GetType() const override;
    
    /**
     * @brief 获取缓冲区大小
     * @return 字节数
     */
    uint32_t GetSize() const override;
    
    /**
     * @brief 获取缓冲区使用模式
     * @return BufferUsage 值
     */
    BufferUsage GetUsage() const override;
    
    /**
     * @brief 绑定顶点缓冲区到当前 GL 上下文
     * @note 必须在有效的 OpenGL 上下文中调用
     */
    void Bind() const override;
    
    /**
     * @brief 解绑顶点缓冲区
     */
    void Unbind() const override;
    
    /**
     * @brief 更新缓冲区数据
     * @param data 新数据指针
     * @param size 新数据大小（不能超过初始大小）
     * @note 仅适用于 Dynamic 或 Stream 缓冲区
     */
    void SetData(const void* data, uint32_t size) override;
    
    /**
     * @brief 获取 OpenGL VBO 句柄
     * @return OpenGL Vertex Buffer Object ID
     */
    uint32_t GetGPUHandle() const override { return m_vbo; }
    
    // ========== 扩展方法 ==========
    
    /**
     * @brief 获取 VAO 句柄
     * @return OpenGL Vertex Array Object ID
     */
    uint32_t GetVAOHandle() const { return m_vao; }
    
    /**
     * @brief 检查缓冲区是否有效
     * @return true 如果缓冲区已成功创建
     */
    bool IsValid() const { return m_vbo != 0 && m_vao != 0; }
    
    /**
     * @brief 获取顶点数量（假设顶点为 float 类型）
     * @param strideInBytes 每个顶点的字节数
     * @return 顶点数量
     */
    uint32_t GetVertexCount(uint32_t strideInBytes) const {
        return strideInBytes > 0 ? m_size / strideInBytes : 0;
    }
    
private:
    uint32_t m_vao = 0;         ///< OpenGL Vertex Array Object
    uint32_t m_vbo = 0;         ///< OpenGL Vertex Buffer Object
    uint32_t m_size = 0;        ///< 缓冲区大小（字节）
    BufferUsage m_usage;        ///< 缓冲区使用模式
    
    /**
     * @brief 内部：创建并初始化 VAO 和 VBO
     * @param data 初始数据
     * @param size 数据大小
     */
    void CreateBuffers(const void* data, uint32_t size);
    
    /**
     * @brief 内部：获取 OpenGL 使用模式标志
     * @return GL_STATIC_DRAW, GL_DYNAMIC_DRAW, 或 GL_STREAM_DRAW
     */
    uint32_t GetGLUsageHint() const;
    
    /**
     * @brief 内部：删除 GPU 资源
     */
    void DestroyBuffers();
};
