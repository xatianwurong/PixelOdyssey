#include "Rectangle.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Rectangle::Rectangle(const glm::vec2& position, const glm::vec4& color, float width, float height)
    : Shape(position, color, 1.0f), m_width(width), m_height(height)
{
    // Generate vertices
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;

    m_vertices = {
        -halfWidth, -halfHeight,  // Bottom left
         halfWidth, -halfHeight,  // Bottom right
         halfWidth,  halfHeight,  // Top right
        -halfWidth,  halfHeight   // Top left
    };

    // Generate indices
    m_indices = {
        0, 1, 2,  // First triangle
        0, 2, 3   // Second triangle
    };
}

void Rectangle::InitOpenGL()
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

void Rectangle::Draw()
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
