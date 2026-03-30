#include "Ellipse.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Ellipse::Ellipse(const glm::vec2& position, const glm::vec4& color, float radiusX, float radiusY, int segments)
    : Shape(position, color, 1.0f), m_radiusX(radiusX), m_radiusY(radiusY), m_segments(segments)
{
    // Generate vertices
    m_vertices.push_back(0.0f);  // Center x
    m_vertices.push_back(0.0f);  // Center y

    for (int i = 0; i <= m_segments; i++)
    {
        float angle = 2.0f * 3.14159265359f * static_cast<float>(i) / static_cast<float>(m_segments);
        float x = m_radiusX * std::cos(angle);
        float y = m_radiusY * std::sin(angle);
        m_vertices.push_back(x);
        m_vertices.push_back(y);
    }

    // Generate indices
    for (int i = 1; i <= m_segments; i++)
    {
        m_indices.push_back(0);      // Center
        m_indices.push_back(i);      // Current vertex
        m_indices.push_back(i + 1);  // Next vertex
    }
}

void Ellipse::InitOpenGL()
{
    if (m_bInitialized)
        return;

    // Create shader
    m_shader = std::make_unique<Shader>("res/shaders/Shape.shader");

    // Create VAO
    CreateVAO();

    // Create vertex buffer
    m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->Bind();

    // Set vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Create index buffer
    m_indexBuffer = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_indexBuffer->Bind();

    // Unbind
    glBindVertexArray(0);
    m_vertexBuffer->Unbind();
    m_indexBuffer->Unbind();

    m_bInitialized = true;
}

void Ellipse::Draw()
{
    if (!m_bInitialized)
        InitOpenGL();

    m_shader->Bind();

    // Set color
    m_shader->SetUniform4f("u_Color", m_color.x, m_color.y, m_color.z, m_color.w);

    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::scale(model, glm::vec3(m_scale, m_scale, 1.0f));
    m_shader->SetUniformMat4f("u_Model", glm::value_ptr(model));

    // Bind VAO
    glBindVertexArray(m_vertexArrayObject);

    // Draw
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    // Unbind
    glBindVertexArray(0);
    m_shader->Unbind();
}
