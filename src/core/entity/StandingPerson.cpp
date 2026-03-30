#include "StandingPerson.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

StandingPerson::StandingPerson(const glm::vec2& position, const glm::vec4& color, float size)
    : Shape(position, color, 1.0f), m_size(size)
{
    // Generate vertices
    float bodyHeight = m_size * 0.4f;
    float headSize = m_size * 0.15f;
    float armLength = m_size * 0.2f;
    float legLength = m_size * 0.3f;
    float width = m_size * 0.1f;

    m_vertices = {
        // Head
        0.0f, bodyHeight,            // Head center
        headSize, bodyHeight,         // Head right
        headSize, bodyHeight + headSize, // Head top right
        0.0f, bodyHeight + headSize,  // Head top
        -headSize, bodyHeight + headSize, // Head top left
        -headSize, bodyHeight,        // Head left
        
        // Body
        0.0f, 0.0f,                  // Body bottom
        0.0f, bodyHeight,            // Body top
        
        // Arms
        0.0f, bodyHeight * 0.7f,      // Right arm top
        armLength, bodyHeight * 0.5f, // Right arm bottom
        0.0f, bodyHeight * 0.7f,      // Left arm top
        -armLength, bodyHeight * 0.5f, // Left arm bottom
        
        // Legs
        0.0f, 0.0f,                  // Right leg top
        width, -legLength,            // Right leg bottom
        0.0f, 0.0f,                  // Left leg top
        -width, -legLength            // Left leg bottom
    };

    // Generate indices
    m_indices = {
        // Head
        0, 1, 2,  // Head right
        0, 2, 3,  // Head top right
        0, 3, 4,  // Head top left
        0, 4, 5,  // Head left
        
        // Body (simplified as a line, but using two triangles for rendering)
        6, 7, 6,  // Body (dummy triangle)
        6, 7, 6,  // Body (dummy triangle)
        
        // Arms
        8, 9, 8,  // Right arm (dummy triangle)
        10, 11, 10, // Left arm (dummy triangle)
        
        // Legs
        12, 13, 12, // Right leg (dummy triangle)
        14, 15, 14  // Left leg (dummy triangle)
    };
}

void StandingPerson::InitOpenGL()
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

void StandingPerson::Draw()
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
