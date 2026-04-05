#pragma once

#include "core/renderer/interfaces/IBuffer.h"
#include <cstdint>

/**
 * @class IndexBufferGL
 * @brief OpenGL 索引缓冲区实现
 * @details
 *   - 管理 OpenGL Element Buffer Object (EBO/IBO)
 *   - 支持索引数据的存储和更新
 *   - 自动追踪索引数量
 *   - 支持高效的索引渲染
 * 
 * @usage
 *   std::vector<uint32_t> indices = { 0, 1, 2, ... };
 *   IndexBufferGL ib(indices.data(), indices.size());
 *   ib.Bind();
 *   // 使用 glDrawElements 进行索引渲染
 * 
 * @thread_safety 不是线程安全的，应在 OpenGL 上下文线程中使用
 */
class IndexBufferGL : public IBuffer {
public:
    /**
     * @brief 构造函数：创建索引缓冲区
     * @param indices 索引数据指针（uint32_t 数组）
     * @param count 索引数量
     * @throw std::runtime_error 如果缓冲区创建失败
     */
    IndexBufferGL(const uint32_t* indices, uint32_t count);
    
    /**
     * @brief 析构函数：释放 GPU 资源
     */
    ~IndexBufferGL();
    
    // ========== IBuffer 接口实现 ==========
    
    /**
     * @brief 获取缓冲区类型
     * @return BufferType::Index
     */
    BufferType GetType() const override;
    
    /**
     * @brief 获取缓冲区大小（字节）
     * @return 字节数（索引数量 * 4）
     */
    uint32_t GetSize() const override;
    
    /**
     * @brief 获取缓冲区使用模式
     * @note 索引缓冲区通常为 Static
     */
    BufferUsage GetUsage() const override;
    
    /**
     * @brief 绑定索引缓冲区到当前 GL 上下文
     * @note 必须在有效的 OpenGL 上下文中调用
     */
    void Bind() const override;
    
    /**
     * @brief 解绑索引缓冲区
     */
    void Unbind() const override;
    
    /**
     * @brief 更新缓冲区数据
     * @param data 新索引数据
     * @param size 新数据大小（字节）
     */
    void SetData(const void* data, uint32_t size) override;
    
    /**
     * @brief 获取 OpenGL EBO 句柄
     * @return OpenGL Element Buffer Object ID
     */
    uint32_t GetGPUHandle() const override { return m_ebo; }
    
    // ========== 扩展方法 ==========
    
    /**
     * @brief 获取索引数量
     * @return 索引元素个数
     */
    uint32_t GetIndexCount() const { return m_count; }
    
    /**
     * @brief 检查缓冲区是否有效
     * @return true 如果缓冲区已成功创建
     */
    bool IsValid() const { return m_ebo != 0; }
    
private:
    uint32_t m_ebo = 0;         ///< OpenGL Element Buffer Object
    uint32_t m_count = 0;       ///< 索引数量
    
    /**
     * @brief 内部：创建并初始化 EBO
     * @param indices 索引数据
     * @param count 索引数量
     */
    void CreateBuffer(const uint32_t* indices, uint32_t count);
    
    /**
     * @brief 内部：删除 GPU 资源
     */
    void DestroyBuffer();
};
