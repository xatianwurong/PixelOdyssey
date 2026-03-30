#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

class Cup : public Shape
{
public:
    Cup(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
        const glm::vec4& color = glm::vec4(0.8f, 0.8f, 1.0f, 1.0f),
        float size = 0.5f);
    
    void Draw() override;

private:
    float m_size;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // Vertex and index data
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};
