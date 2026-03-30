#include "Circle.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Circle::Circle(const glm::vec2& position, const glm::vec4& color, float radius, int segments)
    : Shape(position, color),
      m_radius(radius),
      m_segments(segments)
{
    m_vertices.reserve((segments + 1) * 2);
    m_indices.reserve(segments * 3);
    
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);
    
    float angleStep = static_cast<float>(2.0f * M_PI / segments);
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * angleStep;
        float x = m_radius * cos(angle);
        float y = m_radius * sin(angle);
        m_vertices.push_back(x);
        m_vertices.push_back(y);
    }
    
    for (int i = 1; i <= segments; i++)
    {
        m_indices.push_back(0);
        m_indices.push_back(i);
        m_indices.push_back(i + 1);
    }
}

void Circle::InitOpenGL()
{
    if (m_bInitialized)
        return;
    
    m_shader = std::make_unique<Shader>("res/shaders/Shape.shader");
    
    CreateVAO();
    
    m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->Bind();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    m_indexBuffer = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_indexBuffer->Bind();
    
    glBindVertexArray(0);
    m_vertexBuffer->Unbind();
    m_indexBuffer->Unbind();
    
    m_bInitialized = true;
}

void Circle::Draw()
{
    if (!m_bInitialized)
    {
        InitOpenGL();
    }
    
    m_shader->Bind();
    
    m_shader->SetUniform4f("u_Color", m_color.x, m_color.y, m_color.z, m_color.w);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::scale(model, glm::vec3(m_scale, m_scale, 1.0f));
    m_shader->SetUniformMat4f("u_Model", glm::value_ptr(model));
    
    glBindVertexArray(m_vertexArrayObject);
    
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    
    glBindVertexArray(0);
    m_shader->Unbind();
}

