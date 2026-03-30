#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Triangle::Triangle(const glm::vec2& position, const glm::vec4& color, float size)
  : Shape(position, color),
  m_size(size)
{
  // 预计算顶点和索引数据
  float halfSize = m_size / 2.0f;

  // 等边三角形顶点
  m_vertices = {
      0.0f, halfSize,     // 顶部
      -halfSize, -halfSize, // 左下角
       halfSize, -halfSize  // 右下角
  };

  // 三角形索引
  m_indices = {
      0, 1, 2
  };
}

void Triangle::InitOpenGL()
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

void Triangle::Draw()
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
