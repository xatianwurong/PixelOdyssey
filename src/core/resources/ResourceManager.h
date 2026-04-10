#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Shader;
class Texture;

/**
 * @brief 资源管理器类
 * @details 管理着色器和纹理等资源的加载、获取和卸载
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 提供资源的加载、获取和卸载功能
 * @设计目的 确保资源的统一管理和避免重复加载
 * @使用场景 游戏或应用中的各种资源管理
 * @关键实现 使用哈希表存储资源，支持按名称快速查找
 */
class ResourceManager
{
public:
  /**
   * @brief 获取资源管理器单例实例
   * @return 资源管理器实例引用
   */
  static ResourceManager& Instance();

  /**
   * @brief 析构函数
   */
  ~ResourceManager() = default;

  /**
   * @brief 加载着色器
   * @param name 资源名称
   * @param vertexPath 顶点着色器文件路径
   * @param fragmentPath 片段着色器文件路径
   */
  void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

  /**
   * @brief 加载计算着色器
   * @param name 资源名称
   * @param computePath 计算着色器文件路径
   */
  void LoadShader(const std::string& name, const std::string& computePath);

  /**
   * @brief 获取着色器
   * @param name 资源名称
   * @return 着色器指针，如果不存在返回 nullptr
   */
  Shader* GetShader(const std::string& name);

  /**
   * @brief 卸载着色器
   * @param name 资源名称
   */
  void UnloadShader(const std::string& name);

  /**
   * @brief 加载纹理
   * @param name 资源名称
   * @param path 纹理文件路径
   */
  void LoadTexture(const std::string& name, const std::string& path);

  /**
   * @brief 获取纹理
   * @param name 资源名称
   * @return 纹理指针，如果不存在返回 nullptr
   */
  Texture* GetTexture(const std::string& name);

  /**
   * @brief 卸载纹理
   * @param name 资源名称
   */
  void UnloadTexture(const std::string& name);

  /**
   * @brief 清空所有资源
   * @details 卸载所有已加载的着色器和纹理
   */
  void Clear();

private:
  /**
   * @brief 私有构造函数
   */
  ResourceManager() = default;

  std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;     ///< 着色器资源映射
  std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;   ///< 纹理资源映射
};