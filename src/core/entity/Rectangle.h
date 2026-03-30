#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

/**
 * @class Rectangle
 * @brief 矩形形状类
 * @details 表示矩形形状的类，继承自Shape
 * @功能描述 渲染指定宽度和高度的矩形
 * @设计目的 为渲染矩形提供特定实现
 * @使用场景 用于在场景中渲染矩形形状
 * @关键实现 包含宽度、高度属性和相应的OpenGL渲染逻辑
 */
class Rectangle : public Shape
{
public:
    Rectangle(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
              const glm::vec4& color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
              float width = 1.0f, float height = 0.6f);
    
    void Draw() override;

private:
    float m_width;
    float m_height;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // Vertex and index data
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};