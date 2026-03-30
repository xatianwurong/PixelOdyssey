#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

class Arrow : public Shape
{
public:
    Arrow(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
          const glm::vec4& color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f),
          float length = 1.0f, float width = 0.2f);
    
    void Draw() override;

private:
    float m_length;
    float m_width;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // Vertex and index data
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};
