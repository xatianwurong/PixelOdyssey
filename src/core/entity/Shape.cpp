#include "Shape.h"
#include <GL/glew.h>

Shape::Shape(const glm::vec2& position, const glm::vec4& color, float scale)
    : m_position(position),
      m_color(color),
      m_scale(scale),
      m_vertexArrayObject(0),
      m_bInitialized(false)
{
}

Shape::~Shape()
{
    if (m_vertexArrayObject)
    {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
}

void Shape::SetPosition(const glm::vec2& position)
{
    m_position = position;
}

void Shape::SetColor(const glm::vec4& color)
{
    m_color = color;
}

void Shape::SetScale(float scale)
{
    m_scale = scale;
}

void Shape::CreateVAO()
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
}

