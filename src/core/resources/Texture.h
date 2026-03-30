#pragma once
#include <string>

/**
 * @class Texture
 * @brief 纹理类
 * @details 管理OpenGL纹理资源
 * @功能描述 负责纹理的加载、绑定和管理
 * @设计目的 封装OpenGL纹理操作，提供简洁的接口
 * @使用场景 用于加载和使用纹理资源
 * @关键实现 构造时需要提供纹理文件路径，析构时应释放资源
 */
class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    unsigned int GetID() const { return m_RendererID; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    int m_Width, m_Height, m_Channels;
};