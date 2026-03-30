#pragma once

/**
 * @class IndexBuffer
 * @brief 索引缓冲区类
 * @details 管理OpenGL索引缓冲区对象，用于存储顶点索引数据
 * @功能描述 负责索引缓冲区的创建、绑定和管理
 * @设计目的 封装OpenGL索引缓冲区操作，提供简洁的接口
 * @使用场景 用于存储和传递顶点索引数据给GPU，优化渲染性能
 * @关键实现 构造时创建缓冲区，析构时释放资源，提供Bind/Unbind方法
 */
class IndexBuffer
{
private:
  unsigned int m_RendererID;
  unsigned int m_Count;

public:
  IndexBuffer(const unsigned int* data, int count);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;
  unsigned int GetCount() const { return m_Count; }
};