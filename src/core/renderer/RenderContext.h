#pragma once

#include "interfaces/IRenderContext.h"
#include <glm/glm.hpp>
#include <memory>

class VertexBuffer;
class IndexBuffer;
class IShader;

/**
 * @class RenderContext
 * @brief 渲染上下文实现
 * @details
 *   - 管理OpenGL状态
 *   - 提供矩阵管理
 *   - 支持缓冲区绑定和绘制
 * 
 * @usage
 *   RenderContext ctx;
 *   ctx.SetViewMatrix(camera.GetViewMatrix());
 *   ctx.BindVertexBuffer(vb);
 *   ctx.DrawArrays(vertexCount);
 */
class RenderContext : public IRenderContext {
public:
    RenderContext();
    ~RenderContext() override;
    
    // ========== 矩阵管理 ==========
    
    void SetViewMatrix(const glm::mat4& view) override;
    void SetProjectionMatrix(const glm::mat4& proj) override;
    void SetWorldMatrix(const glm::mat4& world) override;
    
    /**
     * @brief 获取视图投影矩阵的乘积
     * @return VP 矩阵
     */
    glm::mat4 GetViewProjectionMatrix() const;
    
    /**
     * @brief 获取世界视图投影矩阵
     * @return WVP 矩阵
     */
    glm::mat4 GetWorldViewProjectionMatrix() const;
    
    // ========== 资源绑定 ==========
    
    void BindVertexBuffer(IBuffer* vb) override;
    void BindIndexBuffer(IBuffer* ib) override;
    void BindShader(IShader* shader) override;
    
    // ========== 绘制命令 ==========
    
    void DrawIndexed(uint32_t indexCount, uint32_t offset = 0) override;
    void DrawArrays(uint32_t vertexCount, uint32_t offset = 0) override;
    
    // ========== 帧操作 ==========
    
    void Clear(float r, float g, float b, float a) override;
    void Present() override;
    
    // ========== 调试信息 ==========
    
    /**
     * @brief 获取当前绑定的顶点缓冲区
     * @return 指针或nullptr
     */
    IBuffer* GetBoundVertexBuffer() const { return m_currentVB; }
    
    /**
     * @brief 获取当前绑定的索引缓冲区
     * @return 指针或nullptr
     */
    IBuffer* GetBoundIndexBuffer() const { return m_currentIB; }
    
    /**
     * @brief 获取当前绑定的着色器
     * @return 指针或nullptr
     */
    IShader* GetBoundShader() const { return m_currentShader; }
    
private:
    // 矩阵
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_worldMatrix;
    
    // 当前绑定的资源
    IBuffer* m_currentVB = nullptr;
    IBuffer* m_currentIB = nullptr;
    IShader* m_currentShader = nullptr;
    
    // 清除颜色
    struct ClearColor {
        float r, g, b, a;
    } m_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
};
