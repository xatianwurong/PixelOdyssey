#pragma once

#include <cstdint>

/**
 * @brief 缓冲区类型枚举
 * @details 定义 GPU 缓冲区的类型
 */
enum class BufferType
{
  Vertex = 0,  ///< 顶点缓冲区 (VBO)
  Index = 1    ///< 索引缓冲区 (EBO/IBO)
};

/**
 * @brief 缓冲区使用模式枚举
 * @details 影响 GPU 优化和内存分配策略
 */
enum class BufferUsage
{
  Static = 0,   ///< 数据不变，仅上传一次
  Dynamic = 1,  ///< 数据偶尔变化（每秒几次）
  Stream = 2    ///< 数据每帧变化
};

/**
 * @brief 通用缓冲区接口
 * @details 抽象化 GPU 缓冲区操作
 * @details 支持顶点和索引缓冲区
 * @details 隐藏底层 API 实现细节
 * @功能描述 提供统一的缓冲区操作接口
 * @设计目的 抽象化底层图形 API 差异
 * @使用场景 顶点和索引数据管理
 * @关键实现 纯虚接口，由具体图形 API 实现
 */
class IBuffer
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~IBuffer() = default;

  /**
   * @brief 获取缓冲区类型
   * @return BufferType::Vertex 或 BufferType::Index
   */
  virtual BufferType GetType() const = 0;

  /**
   * @brief 获取缓冲区大小
   * @return 缓冲区字节数
   */
  virtual uint32_t GetSize() const = 0;

  /**
   * @brief 获取缓冲区使用模式
   * @return BufferUsage 枚举值
   */
  virtual BufferUsage GetUsage() const = 0;

  /**
   * @brief 绑定缓冲区到当前上下文
   * @details 必须在使用前调用
   */
  virtual void Bind() const = 0;

  /**
   * @brief 解绑缓冲区
   * @details 可选，用于显式释放绑定
   */
  virtual void Unbind() const = 0;

  /**
   * @brief 更新缓冲区数据
   * @param data 新数据指针
   * @param size 数据大小（字节），不能大于初始大小
   * @details 仅适用于 Dynamic 或 Stream 缓冲区
   */
  virtual void SetData(const void* data, uint32_t size) = 0;

  /**
   * @brief 获取 GPU 句柄
   * @return 底层 API 的缓冲区 ID（OpenGL: GLuint，DirectX: ID3D11Buffer*）
   * @details 仅用于高级优化，避免使用
   */
  virtual uint32_t GetGPUHandle() const = 0;
};
