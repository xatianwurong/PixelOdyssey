#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

/**
 * @class Square
 * @brief 正方形实体类
 * @details 表示一个可渲染的正方形对象，继承自Shape
 * @coreResponsibility 负责正方形的初始化和渲染
 * @designIntent 通过顶点缓冲区和索引缓冲区实现正方形的渲染
 * @useCase 用于在OpenGL场景中渲染正方形
 * @attention 使用顶点和索引数据实现正方形的渲染
 */
class Square : public Shape
{
public:
    Square(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
           const glm::vec4& color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
           float size = 1.0f);
    
    void Draw() override;

private:
    float m_size;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // 顶点和索引数据
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};

