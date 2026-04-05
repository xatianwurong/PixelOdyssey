#pragma once

#include "core/renderer/interfaces/IShader.h"
#include "ShaderCompiler.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <cstdint>
#include <string>

/**
 * @class ShaderGL
 * @brief OpenGL 着色器实现
 * @details
 *   - 管理 OpenGL 着色器程序
 *   - 支持 Uniform 变量设置
 *   - 自动 Uniform 位置缓存
 *   - 支持矩阵和向量类型
 * 
 * @usage
 *   ShaderGL shader(vertex_src, fragment_src);
 *   shader.Bind();
 *   shader.SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
 *   shader.SetUniform("matrix", transform_matrix);
 * 
 * @thread_safety 不是线程安全的，应在 OpenGL 上下文线程中使用
 */
class ShaderGL : public IShader {
public:
    /**
     * @brief 构造函数：创建着色器程序
     * @param vertexSource 顶点着色器源代码
     * @param fragmentSource 片段着色器源代码
     * @throw std::runtime_error 如果编译或链接失败
     */
    ShaderGL(const std::string& vertexSource, const std::string& fragmentSource);
    
    /**
     * @brief 析构函数：释放 GPU 资源
     */
    ~ShaderGL();
    
    // ========== IShader 接口实现 ==========
    
    /**
     * @brief 绑定着色器程序到当前 GL 上下文
     */
    void Bind() const override;
    
    /**
     * @brief 解绑着色器程序
     */
    void Unbind() const override;
    
    /**
     * @brief 设置整数 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, int value) override;
    
    /**
     * @brief 设置浮点 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, float value) override;
    
    /**
     * @brief 设置 2D 向量 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, const glm::vec2& value) override;
    
    /**
     * @brief 设置 3D 向量 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, const glm::vec3& value) override;
    
    /**
     * @brief 设置 4D 向量 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, const glm::vec4& value) override;
    
    /**
     * @brief 设置矩阵 Uniform
     * @param name Uniform 变量名
     * @param value 值
     */
    void SetUniform(const char* name, const glm::mat4& value) override;
    
    /**
     * @brief 获取 Uniform 变量位置
     * @param name Uniform 变量名
     * @return OpenGL Uniform 位置，如果不存在返回 -1
     */
    int GetUniformLocation(const char* name) const override;
    
    // ========== 扩展方法 ==========
    
    /**
     * @brief 获取着色器程序 ID
     * @return OpenGL Program ID
     */
    uint32_t GetProgramID() const { return m_program; }
    
    /**
     * @brief 检查着色器是否有效
     * @return true 如果程序已成功创建
     */
    bool IsValid() const { return m_program != 0; }
    
    /**
     * @brief 清除 Uniform 缓存
     * @note 仅在性能调试时使用
     */
    void ClearUniformCache() { m_uniformCache.clear(); }
    
    /**
     * @brief 获取缓存的 Uniform 数量
     * @return 缓存项数
     */
    size_t GetCachedUniformCount() const { return m_uniformCache.size(); }
    
private:
    uint32_t m_program = 0;                              ///< OpenGL Program ID
    mutable std::unordered_map<std::string, int32_t> m_uniformCache;  ///< Uniform 位置缓存
    
    /**
     * @brief 内部：获取或缓存 Uniform 位置
     * @param name Uniform 变量名
     * @return Uniform 位置，或 -1 如果不存在
     */
    int32_t GetCachedUniformLocation(const char* name) const;
    
    /**
     * @brief 内部：删除 GPU 资源
     */
    void DestroyProgram();
};
