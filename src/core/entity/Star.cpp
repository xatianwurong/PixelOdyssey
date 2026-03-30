#include "Star.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Star::Star(const glm::vec2& position, const glm::vec4& color, float size, int points)
    : Shape(position, color),
      m_size(size),
      m_points(points)
{
    // 预计算顶点和索引数据
    float outerRadius = m_size / 2.0f;
    float innerRadius = outerRadius * 0.5f;
    
    m_vertices.reserve(m_points * 2 * 2);
    m_indices.reserve(m_points * 3);
    
    // 中心顶点
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);
    
    // 星形顶点
    float angleStep = 2.0f * M_PI / (m_points * 2);
    for (int i = 0; i < m_points * 2; i++)
    {
        float angle = i * angleStep - M_PI / 2.0f;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        m_vertices.push_back(x);
        m_vertices.push_back(y);
    }
    
    // 三角形索引
    for (int i = 1; i < m_points * 2; i += 2)
    {
        m_indices.push_back(0);
        m_indices.push_back(i);
        m_indices.push_back(i + 1);
        
        if (i + 2 <= m_points * 2)
        {
            m_indices.push_back(0);
            m_indices.push_back(i + 1);
            m_indices.push_back(i + 2);
        }
        else
        {
            m_indices.push_back(0);
            m_indices.push_back(i + 1);
            m_indices.push_back(1);
        }
    }
}

void Star::InitOpenGL()
{
    if (m_bInitialized)
        return;
    
    // 创建着色器
    m_shader = std::make_unique<Shader>("res/shaders/Shape.shader");
    
    // 创建VAO
    CreateVAO();
    
    // 创建顶点缓冲区
    m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->Bind();
    
    // 设置顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    // 创建索引缓冲区
    m_indexBuffer = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_indexBuffer->Bind();
    
    // 解绑
    glBindVertexArray(0);
    m_vertexBuffer->Unbind();
    m_indexBuffer->Unbind();
    
    m_bInitialized = true;
}

void Star::Draw()
{
    if (!m_bInitialized)
    {
        InitOpenGL();
    }
    
    m_shader->Bind();
    
    // 设置颜色
    m_shader->SetUniform4f("u_Color", m_color.x, m_color.y, m_color.z, m_color.w);
    
    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::scale(model, glm::vec3(m_scale, m_scale, 1.0f));
    m_shader->SetUniformMat4f("u_Model", glm::value_ptr(model));
    
    // 绑定VAO
    glBindVertexArray(m_vertexArrayObject);
    
    // 绘制
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    
    // 解绑
    glBindVertexArray(0);
    m_shader->Unbind();
}

