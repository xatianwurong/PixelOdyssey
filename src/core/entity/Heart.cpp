#include "Heart.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Heart::Heart(const glm::vec2& position, const glm::vec4& color, float size, int segments)
    : Shape(position, color),
      m_size(size),
      m_segments(segments)
{
    // 棰勮绠楅《鐐瑰拰绱㈠紩鏁版嵁
    float scale = m_size / 20.0f; // 缂╂斁鍥犲瓙
    
    m_vertices.reserve(m_segments + 1);
    m_indices.reserve(m_segments * 3);
    
    // 涓績椤剁偣
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);
    
    // 蹇冨舰椤剁偣锛堜娇鐢ㄥ弬鏁版柟绋嬶級
    float tStep = 2.0f * M_PI / m_segments;
    for (int i = 0; i <= m_segments; i++)
    {
        float t = i * tStep;
        float x = 16.0f * sin(t) * sin(t) * sin(t);
        float y = 13.0f * cos(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cos(4.0f * t);
        m_vertices.push_back(x * scale);
        m_vertices.push_back(y * scale);
    }
    
    // 涓夎褰㈢储寮?
    for (int i = 1; i <= m_segments; i++)
    {
        m_indices.push_back(0);
        m_indices.push_back(i);
        m_indices.push_back(i + 1);
    }
}

void Heart::InitOpenGL()
{
    if (m_bInitialized)
        return;
    
    // 鍒涘缓鐫€鑹插櫒
    m_shader = std::make_unique<Shader>("res/shaders/Shape.shader");
    
    // 鍒涘缓VAO
    CreateVAO();
    
    // 鍒涘缓椤剁偣缂撳啿鍖?
    m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->Bind();
    
    // 璁剧疆椤剁偣灞炴€?
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    // 鍒涘缓绱㈠紩缂撳啿鍖?
    m_indexBuffer = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_indexBuffer->Bind();
    
    // 瑙ｇ粦
    glBindVertexArray(0);
    m_vertexBuffer->Unbind();
    m_indexBuffer->Unbind();
    
    m_bInitialized = true;
}

void Heart::Draw()
{
    if (!m_bInitialized)
    {
        InitOpenGL();
    }
    
    m_shader->Bind();
    
    // 璁剧疆棰滆壊
    m_shader->SetUniform4f("u_Color", m_color.x, m_color.y, m_color.z, m_color.w);
    
    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::scale(model, glm::vec3(m_scale, m_scale, 1.0f));
    m_shader->SetUniformMat4f("u_Model", glm::value_ptr(model));
    
    // 缁戝畾VAO
    glBindVertexArray(m_vertexArrayObject);
    
    // 缁樺埗
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    
    // 瑙ｇ粦
    glBindVertexArray(0);
    m_shader->Unbind();
}

