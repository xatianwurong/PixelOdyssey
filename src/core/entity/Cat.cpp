#include "Cat.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Cat::Cat(const glm::vec2& position, const glm::vec4& color, float size)
    : Shape(position, color, 1.0f), m_size(size)
{
    // Generate vertices
    float bodySize = m_size * 0.3f;
    float headSize = m_size * 0.2f;
    float earSize = m_size * 0.1f;
    float tailSize = m_size * 0.4f;

    m_vertices = {
        // Body
        0.0f, 0.0f,                  // Body center
        bodySize, 0.0f,              // Body right
        bodySize, -bodySize,         // Body bottom right
        0.0f, -bodySize,             // Body bottom
        -bodySize, -bodySize,        // Body bottom left
        -bodySize, 0.0f,             // Body left
        
        // Head
        0.0f, bodySize,              // Head center
        headSize, bodySize,           // Head right
        headSize, bodySize + headSize, // Head top right
        0.0f, bodySize + headSize,    // Head top
        -headSize, bodySize + headSize, // Head top left
        -headSize, bodySize,          // Head left
        
        // Ears
        0.0f, bodySize + headSize,    // Head top (ear base)
        headSize * 0.5f, bodySize + headSize + earSize, // Right ear
        -headSize * 0.5f, bodySize + headSize + earSize, // Left ear
        
        // Tail
        bodySize, 0.0f,              // Body right (tail base)
        bodySize + tailSize, 0.0f,    // Tail end
        bodySize + tailSize * 0.5f, -tailSize * 0.3f // Tail curve
    };

    // Generate indices
    m_indices = {
        // Body
        0, 1, 2,  // Body right
        0, 2, 3,  // Body bottom right
        0, 3, 4,  // Body bottom left
        0, 4, 5,  // Body left
        
        // Head
        6, 7, 8,  // Head right
        6, 8, 9,  // Head top right
        6, 9, 10, // Head top left
        6, 10, 11, // Head left
        
        // Ears
        12, 13, 8, // Right ear
        12, 10, 14, // Left ear
        
        // Tail
        15, 16, 17 // Tail
    };
}

void Cat::InitOpenGL()
{
    if (m_bInitialized)
        return;

    // Create shader
    m_shader = std::make_unique<Shader>("d:\\TestOPenGL\\res\\shaders\\Shape.shader");

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

void Cat::Draw()
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
