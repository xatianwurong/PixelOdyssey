#include "Pentagram.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Pentagram::Pentagram(const glm::vec2& position, const glm::vec4& color, float size)
    : Shape(position, color),
      m_size(size)
{
    // 棰勮绠楅《鐐瑰拰绱㈠紩鏁版嵁
    float outerRadius = m_size / 2.0f;
    float innerRadius = outerRadius * 0.382f; // 榛勯噾姣斾緥
    
    // 浜旇鏄熼《鐐?
    m_vertices = {
        0.0f, outerRadius,                       // 椤堕儴
        -innerRadius * 0.5f, innerRadius * 0.866f,  // 宸︿笂鍐呯偣
        -outerRadius * 0.866f, outerRadius * 0.5f,  // 宸︿笂澶栫偣
        -outerRadius * 0.309f, -outerRadius * 0.951f, // 宸︿笅澶栫偣
        0.0f, -innerRadius,                      // 涓績涓嬬偣
        outerRadius * 0.309f, -outerRadius * 0.951f,  // 鍙充笅澶栫偣
        outerRadius * 0.866f, outerRadius * 0.5f,   // 鍙充笂澶栫偣
        innerRadius * 0.5f, innerRadius * 0.866f    // 鍙充笂鍐呯偣
    };
    
    // 涓夎褰㈢储寮?
    m_indices = {
        0, 2, 4,
        0, 4, 6,
        2, 1, 3,
        4, 3, 5,
        6, 5, 7,
        0, 7, 2,
        0, 6, 7
    };
}

void Pentagram::InitOpenGL()
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

void Pentagram::Draw()
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

