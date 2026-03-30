#pragma once

/**
 * @class VertexBuffer
 * @brief 顶点缓冲区类
 * @details 管理OpenGL顶点缓冲区对象，用于存储顶点数据
 * @coreResponsibility 负责顶点缓冲区的创建、绑定和管理
 * @designIntent 封装OpenGL顶点缓冲区操作，提供简洁的接口
 * @useCase 用于存储和传递顶点数据给GPU
 * @attention 构造时需要提供顶点数据和大小，析构时应释放资源
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
