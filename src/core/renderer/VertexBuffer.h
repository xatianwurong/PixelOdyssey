#pragma once

/**
 * @class VertexBuffer
 * @brief 顶点缓冲区类
 * @details 管理OpenGL顶点缓冲区对象，用于存储顶点数据
 * @功能描述 负责顶点缓冲区的创建、绑定和管理
 * @设计目的 封装OpenGL顶点缓冲区操作，提供简洁的接口
 * @使用场景 用于存储和传递顶点数据给GPU
 * @关键实现 构造时创建缓冲区，析构时释放资源，提供Bind/Unbind方法
 */
class VertexBuffer
{
  unsigned int m_RendererID;
public:
  VertexBuffer(const void* data, int size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};
