#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

/**
 * @class Star
 * @brief 星形实体类
 * @details 表示一个可渲染的星形对象，继承自Shape
 * @coreResponsibility 负责星形的初始化和渲染
 * @designIntent 通过顶点缓冲区和索引缓冲区实现星形的渲染
 * @useCase 用于在OpenGL场景中渲染星形
 * @attention 使用顶点和索引数据实现星形的渲染
 */
class Star : public Shape
{
public:
    Star(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
         const glm::vec4& color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f),
         float size = 1.0f, int points = 5);
    
    void Draw() override;

private:
    float m_size;
    int m_points;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // 顶点和索引数据
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};

