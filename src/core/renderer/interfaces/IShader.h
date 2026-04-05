#pragma once

#include <glm/glm.hpp>

/**
 * @brief 着色器程序接口
 * @details 抽象化顶点和片段着色器
 * @details 支持 uniform 参数设置
 * @details 隐藏底层 GLSL/HLSL 实现
 * @功能描述 提供统一的着色器操作接口
 * @设计目的 抽象化底层着色器实现
 * @使用场景 渲染管线中的着色器管理
 * @关键实现 纯虚接口，由具体图形 API 实现
 */
class IShader
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~IShader() = default;

  /**
   * @brief 绑定着色器程序
   * @details 后续绘制调用将使用此着色器
   */
  virtual void Bind() const = 0;

  /**
   * @brief 解绑着色器程序
   */
  virtual void Unbind() const = 0;

  // ========== Uniform 设置 ==========

  /**
   * @brief 设置整数 uniform
   * @param name uniform 变量名（如 "uLayer"）
   * @param value 整数值
   */
  virtual void SetUniform(const char* name, int value) = 0;

  /**
   * @brief 设置浮点数 uniform
   * @param name uniform 变量名
   * @param value 浮点值
   */
  virtual void SetUniform(const char* name, float value) = 0;

  /**
   * @brief 设置向量 2 uniform
   * @param name uniform 变量名
   * @param value 2 维向量
   */
  virtual void SetUniform(const char* name, const glm::vec2& value) = 0;

  /**
   * @brief 设置向量 3 uniform
   * @param name uniform 变量名
   * @param value 3 维向量
   */
  virtual void SetUniform(const char* name, const glm::vec3& value) = 0;

  /**
   * @brief 设置向量 4 uniform
   * @param name uniform 变量名
   * @param value 4 维向量
   */
  virtual void SetUniform(const char* name, const glm::vec4& value) = 0;

  /**
   * @brief 设置矩阵 4 uniform
   * @param name uniform 变量名
   * @param value 4x4 矩阵
   */
  virtual void SetUniform(const char* name, const glm::mat4& value) = 0;

  /**
   * @brief 获取 uniform 位置
   * @param name uniform 变量名
   * @return uniform 位置，-1 表示不存在
   */
  virtual int GetUniformLocation(const char* name) const = 0;
};
