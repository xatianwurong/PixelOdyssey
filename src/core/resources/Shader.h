#pragma once
#include <string>

/**
 * @class Shader
 * @brief 着色器类
 * @details 管理OpenGL着色器程序，包括顶点着色器、片段着色器和计算着色器
 * @coreResponsibility 负责着色器的编译、链接和管理
 * @designIntent 封装OpenGL着色器操作，提供简洁的接口
 * @useCase 用于加载、编译和使用着色器程序
 * @attention 构造时需要提供着色器文件路径，析构时应释放资源
 */
class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;

public:
    Shader(const std::string& filePath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniform variables
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMatrix4fv(const std::string& name, int count, bool transpose, const float* value);
    void SetUniformMat4f(const std::string& name, const float* value);

    int GetUniformLocation(const std::string& name);

private:
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CreateComputeShader(const std::string& computeShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    std::string ParseShader(const std::string& filePath);
};
