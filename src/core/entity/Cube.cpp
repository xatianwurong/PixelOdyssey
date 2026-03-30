#include "Cube.h"
#include <GL/GLEW.h>
#include <iostream>

Cube::Cube()
    : m_vertexArrayObject(0)
{
    std::cout << "Cube constructor called!" << std::endl;
    std::cout << "Creating Shader object..." << std::endl;
    m_shader = std::make_unique<Shader>("d:\\TestOPenGL\\res\\shaders\\Cube.shader");
    std::cout << "Shader object created!" << std::endl;

    // 立方体的8个顶点
    m_vertexPositionBufferData = {
        -1.0f, -1.0f, -1.0f, // 0
        -1.0f, -1.0f,  1.0f, // 1
        -1.0f,  1.0f,  1.0f, // 2
        -1.0f,  1.0f, -1.0f, // 3
         1.0f, -1.0f, -1.0f, // 4
         1.0f, -1.0f,  1.0f, // 5
         1.0f,  1.0f,  1.0f, // 6
         1.0f,  1.0f, -1.0f  // 7
    };

    // 立方体的12个三角形索引
    m_indexBufferData = {
        0, 1, 2,   0, 2, 3,  // 正面
        4, 5, 6,   4, 6, 7,  // 背面
        0, 1, 5,   0, 5, 4,  // 底面
        3, 2, 6,   3, 6, 7,  // 顶面
        0, 3, 7,   0, 7, 4,  // 左面
        1, 2, 6,   1, 6, 5   // 右面
    };

    // 每个顶点一个RGB三元组
    m_colorBufferData = {
        0.583f,  0.771f,  0.014f, // 0
        0.609f,  0.115f,  0.436f, // 1
        0.327f,  0.483f,  0.844f, // 2
        0.822f,  0.569f,  0.201f, // 3
        0.435f,  0.602f,  0.223f, // 4
        0.310f,  0.747f,  0.185f, // 5
        0.597f,  0.770f,  0.761f, // 6
        0.559f,  0.436f,  0.730f  // 7
    };

    // Initialize camera
    m_camera = std::make_unique<Camera>();

    // Defer OpenGL initialization until first draw
    m_bInitialized = false;
}

Cube::~Cube()
{
    if (m_vertexArrayObject != 0)
    {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
}

void Cube::InitOpenGL()
{
    std::cout << "Initializing OpenGL resources for Cube..." << std::endl;
    
    // Create buffers
    m_positionBuffer = std::make_unique<VertexBuffer>(m_vertexPositionBufferData.data(), static_cast<int>(m_vertexPositionBufferData.size() * sizeof(float)));
    m_colorBuffer = std::make_unique<VertexBuffer>(m_colorBufferData.data(), static_cast<int>(m_colorBufferData.size() * sizeof(float)));
    m_indexBuffer = std::make_unique<IndexBuffer>(m_indexBufferData.data(), static_cast<int>(m_indexBufferData.size()));

    // Create and configure VAO
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Bind position buffer and configure vertex attribute 0
    m_positionBuffer->Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Bind color buffer and configure vertex attribute 1
    m_colorBuffer->Bind();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Bind index buffer
    m_indexBuffer->Bind();

    // Unbind VAO
    glBindVertexArray(0);

    m_bInitialized = true;
    std::cout << "OpenGL resources initialized successfully!" << std::endl;
}

void Cube::Draw()
{
    // Initialize OpenGL resources if not already done
    if (!m_bInitialized)
    {
        InitOpenGL();
    }

    // Get view projection matrix from camera
    glm::mat4 viewProjection = m_camera->GetViewProjectionMatrix();
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = viewProjection * model; // Remember, matrix multiplication is the other way around

    // Use our shader
    m_shader->Bind();
    m_shader->SetUniformMatrix4fv("MVP", 1, GL_FALSE, &mvp[0][0]);

    // Bind VAO
    glBindVertexArray(m_vertexArrayObject);

    // Draw the cube using indices
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 12 triangles * 3 vertices each

    // Unbind VAO
    glBindVertexArray(0);
}

