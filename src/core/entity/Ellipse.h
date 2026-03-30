#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

class Ellipse : public Shape
{
public:
    Ellipse(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
            const glm::vec4& color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
            float radiusX = 0.5f,
            float radiusY = 0.3f,
            int segments = 32);
    
    void Draw() override;

private:
    float m_radiusX;
    float m_radiusY;
    int m_segments;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // Vertex and index data
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};
