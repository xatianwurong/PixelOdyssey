#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Shader;
class Texture;

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