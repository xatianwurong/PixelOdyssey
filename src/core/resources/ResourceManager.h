#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Shader;
class Texture;

/**
 * @class ResourceManager
 * @brief 资源管理器类
 * @details 管理着色器和纹理等资源的加载、获取和卸载
 * @功能描述 提供资源的加载、获取和卸载功能
 * @设计目的 使用单例模式，确保资源的统一管理和避免重复加载
 * @使用场景 用于管理游戏或应用中的各种资源
 * @关键实现 使用单例模式，通过Instance()方法获取实例
 */
class ResourceManager
{
public:
    static ResourceManager& Instance();
    ~ResourceManager() = default;

    void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void LoadShader(const std::string& name, const std::string& computePath);
    Shader* GetShader(const std::string& name);
    void UnloadShader(const std::string& name);

    void LoadTexture(const std::string& name, const std::string& path);
    Texture* GetTexture(const std::string& name);
    void UnloadTexture(const std::string& name);

    void Clear();

private:
    ResourceManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};