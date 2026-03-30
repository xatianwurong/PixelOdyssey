#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    if (m_shaders.find(name) == m_shaders.end())
    {
        m_shaders[name] = std::make_unique<Shader>(vertexPath, fragmentPath);
    }
}

void ResourceManager::LoadShader(const std::string& name, const std::string& computePath)
{
    if (m_shaders.find(name) == m_shaders.end())
    {
        m_shaders[name] = std::make_unique<Shader>(computePath);
    }
}

Shader* ResourceManager::GetShader(const std::string& name)
{
    auto it = m_shaders.find(name);
    return it != m_shaders.end() ? it->second.get() : nullptr;
}

void ResourceManager::UnloadShader(const std::string& name)
{
    m_shaders.erase(name);
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
    if (m_textures.find(name) == m_textures.end())
    {
        // TODO: Implement texture loading
    }
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    return it != m_textures.end() ? it->second.get() : nullptr;
}

void ResourceManager::UnloadTexture(const std::string& name)
{
    m_textures.erase(name);
}

void ResourceManager::Clear()
{
    m_shaders.clear();
    m_textures.clear();
}