#include "RenderContext.h"
#include "../logging/Logger.h"
#include "interfaces/IBuffer.h"
#include "interfaces/IShader.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Logging;

/**
 * @brief 构造函数
 * @details 初始化渲染上下文的矩阵
 */
RenderContext::RenderContext()
    : m_viewMatrix(1.0f), m_projectionMatrix(1.0f), m_worldMatrix(1.0f)
{
    LOG_DEBUG("RenderContext", "RenderContext created");
}

/**
 * @brief 析构函数
 * @details 清理渲染上下文资源
 */
RenderContext::~RenderContext()
{
    LOG_DEBUG("RenderContext", "RenderContext destroyed");
}

/**
 * @brief 设置视图矩阵
 * @param view 视图矩阵
 * @details 设置相机的视图变换矩阵
 */
void RenderContext::SetViewMatrix(const glm::mat4& view)
{
    m_viewMatrix = view;
}

/**
 * @brief 设置投影矩阵
 * @param proj 投影矩阵
 * @details 设置相机的投影变换矩阵
 */
void RenderContext::SetProjectionMatrix(const glm::mat4& proj)
{
    m_projectionMatrix = proj;
}

/**
 * @brief 设置世界矩阵
 * @param world 世界矩阵
 * @details 设置对象的世界变换矩阵
 */
void RenderContext::SetWorldMatrix(const glm::mat4& world)
{
    m_worldMatrix = world;
}

/**
 * @brief 获取视图投影矩阵
 * @return 视图矩阵与投影矩阵的乘积
 * @details 用于将世界坐标转换到裁剪空间
 */
glm::mat4 RenderContext::GetViewProjectionMatrix() const
{
    return m_projectionMatrix * m_viewMatrix;
}

/**
 * @brief 获取世界视图投影矩阵
 * @return 世界矩阵、视图矩阵和投影矩阵的乘积
 * @details 用于将模型坐标转换到裁剪空间
 */
glm::mat4 RenderContext::GetWorldViewProjectionMatrix() const
{
    return m_projectionMatrix * m_viewMatrix * m_worldMatrix;
}

/**
 * @brief 绑定顶点缓冲区
 * @param vb 顶点缓冲区指针
 * @details 如果缓冲区已绑定则不重复绑定
 */
void RenderContext::BindVertexBuffer(IBuffer* vb)
{
    if (m_currentVB != vb)
    {
        if (vb)
        {
            vb->Bind();
            m_currentVB = vb;
        }
        else
        {
            LOG_WARNING("RenderContext", "Attempting to bind null vertex buffer");
        }
    }
}

/**
 * @brief 绑定索引缓冲区
 * @param ib 索引缓冲区指针
 * @details 如果缓冲区已绑定则不重复绑定
 */
void RenderContext::BindIndexBuffer(IBuffer* ib)
{
    if (m_currentIB != ib)
    {
        if (ib)
        {
            ib->Bind();
            m_currentIB = ib;
        }
        else
        {
            LOG_WARNING("RenderContext", "Attempting to bind null index buffer");
        }
    }
}

/**
 * @brief 绑定着色器
 * @param shader 着色器指针
 * @details 如果着色器已绑定则不重复绑定
 */
void RenderContext::BindShader(IShader* shader)
{
    if (m_currentShader != shader)
    {
        if (shader)
        {
            shader->Bind();
            m_currentShader = shader;
        }
        else
        {
            LOG_WARNING("RenderContext", "Attempting to bind null shader");
        }
    }
}

/**
 * @brief 执行索引绘制
 * @param indexCount 索引数量
 * @param offset 起始偏移量
 * @details 使用绑定的顶点和索引缓冲区进行绘制
 */
void RenderContext::DrawIndexed(uint32_t indexCount, uint32_t offset)
{
    if (!m_currentVB || !m_currentIB)
    {
        LOG_WARNING("RenderContext", "DrawIndexed called without binding VB or IB");
        return;
    }

    if (indexCount == 0)
    {
        LOG_WARNING("RenderContext", "DrawIndexed called with zero index count");
        return;
    }

    // 这里应该调用 OpenGL 的绘制函数
    // glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
    LOG_DEBUG("RenderContext", "DrawIndexed: count=" + std::to_string(indexCount) +
              ", offset=" + std::to_string(offset));
}

/**
 * @brief 执行数组绘制
 * @param vertexCount 顶点数量
 * @param offset 起始偏移量
 * @details 使用绑定的顶点缓冲区进行绘制
 */
void RenderContext::DrawArrays(uint32_t vertexCount, uint32_t offset)
{
    if (!m_currentVB)
    {
        LOG_WARNING("RenderContext", "DrawArrays called without binding VB");
        return;
    }

    if (vertexCount == 0)
    {
        LOG_WARNING("RenderContext", "DrawArrays called with zero vertex count");
        return;
    }

    // 这里应该调用 OpenGL 的绘制函数
    // glDrawArrays(GL_TRIANGLES, offset, vertexCount);
    LOG_DEBUG("RenderContext", "DrawArrays: count=" + std::to_string(vertexCount) +
              ", offset=" + std::to_string(offset));
}

/**
 * @brief 清空渲染目标
 * @param r 红色分量（0-1）
 * @param g 绿色分量（0-1）
 * @param b 蓝色分量（0-1）
 * @param a 透明度分量（0-1）
 * @details 使用指定颜色清空屏幕
 */
void RenderContext::Clear(float r, float g, float b, float a)
{
    m_clearColor = {r, g, b, a};
    // 这里应该调用 OpenGL 的清除函数
    // glClearColor(r, g, b, a);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LOG_DEBUG("RenderContext", "Clear: color=(" + std::to_string(r) + "," +
              std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a) + ")");
}

/**
 * @brief 呈现帧缓冲区
 * @details 交换前后缓冲区，显示渲染结果
 */
void RenderContext::Present()
{
    // 这里应该交换缓冲区
    // SwapBuffers(...)
    LOG_DEBUG("RenderContext", "Present");
}
