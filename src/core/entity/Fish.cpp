#include "Fish.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Fish::Fish(const glm::vec2& position, const glm::vec4& color, float size)
    : Shape(position, color, 1.0f), m_size(size)
{
    // Generate vertices
    float bodyLength = m_size;
    float bodyHeight = m_size * 0.4f;
    float tailLength = m_size * 0.3f;
    float finHeight = m_size * 0.2f;

    m_vertices = {
        // Body
        0.0f, 0.0f,                  // Body front
        bodyLength, 0.0f,            // Body back
        bodyLength, -bodyHeight/2,    // Body bottom back
        0.0f, -bodyHeight/2,          // Body bottom front
        0.0f, bodyHeight/2,           // Body top front
        bodyLength, bodyHeight/2,      // Body top back
        
        // Tail
        bodyLength, 0.0f,            // Tail base
        bodyLength + tailLength, 0.0f, // Tail end
        bodyLength + tailLength/2, -tailLength/2, // Tail bottom
        bodyLength + tailLength/2, tailLength/2,  // Tail top
        
        // Fins
        bodyLength/3, 0.0f,           // Dorsal fin base
        bodyLength/3, bodyHeight/2 + finHeight, // Dorsal fin top
        bodyLength*2/3, 0.0f,         // Pectoral fin base
        bodyLength*2/3, -bodyHeight/2 - finHeight, // Pectoral fin bottom
        bodyLength/2, -bodyHeight/2,  // Pelvic fin base
        bodyLength/2, -bodyHeight/2 - finHeight/2 // Pelvic fin bottom
    };

    // Generate indices
    m_indices = {
        // Body
        0, 1, 2,  // Body bottom
        0, 2, 3,  // Body bottom
        0, 1, 5,  // Body top
        0, 5, 4,  // Body top
        
        // Tail
        6, 7, 8,  // Tail bottom
        6, 9, 7,  // Tail top
        
        // Fins
        10, 11, 4, // Dorsal fin
        12, 13, 3, // Pectoral fin
        14, 15, 3  // Pelvic fin
    };
}

void Fish::InitOpenGL()
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

void Fish::Draw()
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
