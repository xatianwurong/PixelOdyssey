#pragma once

#include <glm/glm.hpp>
#include <cstdint>

// Forward declarations
class IBuffer;  ///< 缓冲区接口前向声明
class IShader;  ///< 着色器接口前向声明

/**
 * @brief 渲染上下文接口
 * @details 抽象化底层图形 API（OpenGL、DirectX 等）
 * @details 提供统一的矩阵管理和绘制命令
 * @details 支持状态管理和资源绑定
 * @功能描述 提供统一的渲染操作接口
 * @设计目的 抽象化底层图形 API 差异
 * @使用场景 渲染管线中的上下文管理
 * @关键实现 纯虚接口，由具体图形 API 实现
 */
class IRenderContext
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~IRenderContext() = default;

  // ========== 矩阵管理 ==========

  /**
   * @brief 设置视图矩阵（观察矩阵）
   * @param view 4x4 视图矩阵
   * @details 通常从摄像机获取
   */
  virtual void SetViewMatrix(const glm::mat4& view) = 0;

  /**
   * @brief 设置投影矩阵
   * @param proj 4x4 投影矩阵
   * @details 透视投影或正交投影
   */
  virtual void SetProjectionMatrix(const glm::mat4& proj) = 0;

  /**
   * @brief 设置世界变换矩阵
   * @param world 4x4 世界矩阵
   * @details 每个对象的变换矩阵
   */
  virtual void SetWorldMatrix(const glm::mat4& world) = 0;

  // ========== 资源绑定 ==========

  /**
   * @brief 绑定顶点缓冲区
   * @param vb 顶点缓冲区指针
   */
  virtual void BindVertexBuffer(IBuffer* vb) = 0;

  /**
   * @brief 绑定索引缓冲区
   * @param ib 索引缓冲区指针
   */
  virtual void BindIndexBuffer(IBuffer* ib) = 0;

  /**
   * @brief 绑定着色器程序
   * @param shader 着色器程序指针
   */
  virtual void BindShader(IShader* shader) = 0;

  // ========== 绘制命令 ==========

  /**
   * @brief 使用索引缓冲区绘制
   * @param indexCount 要绘制的索引数量
   * @param offset 起始偏移（以索引计）
   * @details 必须先调用 BindIndexBuffer 和 BindVertexBuffer
   */
  virtual void DrawIndexed(uint32_t indexCount, uint32_t offset = 0) = 0;

  /**
   * @brief 使用顶点数组绘制
   * @param vertexCount 要绘制的顶点数量
   * @param offset 起始偏移（以顶点计）
   * @details 必须先调用 BindVertexBuffer
   */
  virtual void DrawArrays(uint32_t vertexCount, uint32_t offset = 0) = 0;

  // ========== 帧操作 ==========

  /**
   * @brief 清除帧缓冲区
   * @param r 红色分量 (0.0-1.0)
   * @param g 绿色分量 (0.0-1.0)
   * @param b 蓝色分量 (0.0-1.0)
   * @param a 透明度分量 (0.0-1.0)
   * @details 通常在每帧开始时调用
   */
  virtual void Clear(float r, float g, float b, float a) = 0;

  /**
   * @brief 提交帧到显示器
   * @details 交换前后缓冲区，显示渲染结果
   */
  virtual void Present() = 0;
};
