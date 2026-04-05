#pragma once

#include <glm/glm.hpp>

/**
 * @interface IShader
 * @brief 着色器程序接口
 * @details
 *   - 抽象化顶点和片段着色器
 *   - 支持 uniform 参数设置
 *   - 隐藏底层GLSL/HLSL实现
 * 
 * @usage
 *   IShader* shader = CreateShaderProgram(vsSource, fsSource);
 *   shader->Bind();
 *   shader->SetUniform("uColor", glm::vec4(1, 0, 0, 1));
 *   // 渲染
 *   shader->Unbind();
 */
class IShader {
public:
    virtual ~IShader() = default;
    
    /**
     * @brief 绑定着色器程序
     * @note 后续绘制调用将使用此着色器
     */
    virtual void Bind() const = 0;
    
    /**
     * @brief 解绑着色器程序
     */
    virtual void Unbind() const = 0;
    
    // ========== Uniform 设置 ==========
    
    /**
     * @brief 设置整数 uniform
     * @param name uniform 变量名（如 "uLayer"）
     * @param value 整数值
     */
    virtual void SetUniform(const char* name, int value) = 0;
    
    /**
     * @brief 设置浮点数 uniform
     * @param name uniform 变量名
     * @param value 浮点值
     */
    virtual void SetUniform(const char* name, float value) = 0;
    
    /**
     * @brief 设置向量2 uniform
     * @param name uniform 变量名
     * @param value 2维向量
     */
    virtual void SetUniform(const char* name, const glm::vec2& value) = 0;
    
    /**
     * @brief 设置向量3 uniform
     * @param name uniform 变量名
     * @param value 3维向量
     */
    virtual void SetUniform(const char* name, const glm::vec3& value) = 0;
    
    /**
     * @brief 设置向量4 uniform
     * @param name uniform 变量名
     * @param value 4维向量
     */
    virtual void SetUniform(const char* name, const glm::vec4& value) = 0;
    
    /**
     * @brief 设置矩阵4 uniform
     * @param name uniform 变量名
     * @param value 4x4矩阵
     */
    virtual void SetUniform(const char* name, const glm::mat4& value) = 0;
    
    /**
     * @brief 获取 uniform 位置
     * @param name uniform 变量名
     * @return uniform 位置，-1 表示不存在
     */
    virtual int GetUniformLocation(const char* name) const = 0;
};
