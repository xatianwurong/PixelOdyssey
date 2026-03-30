#include "Bird.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Bird::Bird(const glm::vec2& position, const glm::vec4& color, float size)
  : Shape(position, color, 1.0f), m_size(size)
{
  // Generate vertices
  float bodySize = m_size * 0.2f;
  float headSize = m_size * 0.1f;
  float beakSize = m_size * 0.05f;
  float wingSize = m_size * 0.3f;
  float tailSize = m_size * 0.2f;

  m_vertices = {
    // Body
    0.0f, 0.0f,                  // Body center
    bodySize, 0.0f,              // Body right
    bodySize, -bodySize,         // Body bottom right
    0.0f, -bodySize,             // Body bottom

    // Head
    0.0f, 0.0f,                  // Head center (connected to body)
    -headSize, 0.0f,             // Head left
    -headSize, headSize,         // Head top left
    0.0f, headSize,              // Head top

    // Beak
    -headSize, 0.0f,             // Beak base
    -headSize - beakSize, 0.0f,  // Beak tip
    -headSize, -beakSize / 2,      // Beak bottom

    // Wing
    bodySize / 2, 0.0f,            // Wing base
    bodySize / 2 + wingSize, -wingSize, // Wing tip
    bodySize / 2, -wingSize / 2,     // Wing bottom

    // Tail
    bodySize, 0.0f,              // Tail base
    bodySize + tailSize, 0.0f,    // Tail end
    bodySize + tailSize / 2, -tailSize / 2 // Tail bottom
  };

  // Generate indices
  m_indices = {
    // Body
    0, 1, 2,  // Body right
    0, 2, 3,  // Body bottom

    // Head
    4, 5, 6,  // Head left
    4, 6, 7,  // Head top

    // Beak
    8, 9, 10, // Beak

    // Wing
    11, 12, 13, // Wing

    // Tail
    14, 15, 16 // Tail
  };
}

void Bird::InitOpenGL()
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

void Bird::Draw()
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
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

  // Unbind
  glBindVertexArray(0);
  m_shader->Unbind();
}
