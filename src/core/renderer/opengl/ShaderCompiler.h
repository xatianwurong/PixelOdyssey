#pragma once

#include <cstdint>
#include <string>

/**
 * @class ShaderCompiler
 * @brief OpenGL 着色器编译工具
 * @details
 *   - 编译顶点和片段着色器
 *   - 链接着色器程序
 *   - 完整的错误报告
 *   - 日志记录编译过程
 * 
 * @usage
 *   const char* vs_src = "...";
 *   const char* fs_src = "...";
 *   uint32_t program = ShaderCompiler::CompileProgram(vs_src, fs_src);
 * 
 * @thread_safety 不是线程安全的，应在 OpenGL 上下文线程中使用
 */
class ShaderCompiler {
public:
    /**
     * @brief 编译单个着色器
     * @param type 着色器类型 (GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER)
     * @param source 着色器源代码
     * @return OpenGL 着色器 ID
     * @throw std::runtime_error 如果编译失败
     */
    static uint32_t CompileShader(uint32_t type, const std::string& source);
    
    /**
     * @brief 编译完整的着色器程序
     * @param vertexSource 顶点着色器源代码
     * @param fragmentSource 片段着色器源代码
     * @return OpenGL 程序 ID
     * @throw std::runtime_error 如果编译或链接失败
     */
    static uint32_t CompileProgram(const std::string& vertexSource, 
                                  const std::string& fragmentSource);
    
    /**
     * @brief 获取着色器编译错误信息
     * @param shader 着色器 ID
     * @return 错误信息字符串
     */
    static std::string GetCompileError(uint32_t shader);
    
    /**
     * @brief 获取程序链接错误信息
     * @param program 程序 ID
     * @return 错误信息字符串
     */
    static std::string GetLinkError(uint32_t program);
    
    /**
     * @brief 验证着色器源代码格式
     * @param source 源代码
     * @return true 如果源代码看起来有效
     */
    static bool ValidateSource(const std::string& source);
};
