#include "Car.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Car::Car(const glm::vec2& position, const glm::vec4& color, float size)
    : Shape(position, color, 1.0f), m_size(size)
{
    // Generate vertices
    float length = m_size;
    float width = m_size * 0.4f;
    float height = m_size * 0.3f;
    float wheelRadius = m_size * 0.1f;

    m_vertices = {
        // Car body
        -length/2, -width/2,         // Bottom left
         length/2, -width/2,         // Bottom right
         length/2 - length*0.1f, 0.0f,  // Front top right
        -length/2 + length*0.2f, 0.0f,  // Front top left
        -length/2 + length*0.3f, height, // Rear top left
         length/2 - length*0.2f, height, // Rear top right
        
        // Wheels (simplified as circles)
        -length/4, -width/2 - wheelRadius,  // Front wheel center
         length/4, -width/2 - wheelRadius   // Rear wheel center
    };

    // Generate indices
    m_indices = {
        0, 1, 2,  // Body front
        0, 2, 3,  // Body front
        3, 2, 5,  // Body top
        3, 5, 4,  // Body top
        4, 5, 1,  // Body rear
        4, 1, 0   // Body rear
    };
}

void Car::InitOpenGL()
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

void Car::Draw()
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

    // Draw body
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    // Unbind
    glBindVertexArray(0);
    m_shader->Unbind();
}
