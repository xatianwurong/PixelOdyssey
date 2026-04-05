#pragma once
#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>

/**
 * @brief 场景管理器类
 * @details 管理多个场景的切换和操作
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 提供场景的添加、移除、获取和切换功能
 * @设计目的 确保场景的统一管理
 * @使用场景 游戏或应用中的多个场景管理
 * @关键实现 使用哈希表存储场景，支持按名称快速查找
 */
class SceneManager
{
public:
  /**
   * @brief 获取场景管理器单例实例
   * @return 场景管理器实例引用
   */
  static SceneManager& Instance();

  /**
   * @brief 析构函数
   */
  ~SceneManager() = default;

  /**
   * @brief 添加场景
   * @param scene 场景指针
   */
  void AddScene(Scene* scene);

  /**
   * @brief 移除场景
   * @param name 场景名称
   */
  void RemoveScene(const std::string& name);

  /**
   * @brief 获取场景
   * @param name 场景名称
   * @return 场景指针，如果不存在返回 nullptr
   */
  Scene* GetScene(const std::string& name);

  /**
   * @brief 设置激活场景
   * @param name 场景名称
   */
  void SetActiveScene(const std::string& name);

  /**
   * @brief 获取激活场景
   * @return 当前激活的场景指针，可能为 nullptr
   */
  Scene* GetActiveScene();

  /**
   * @brief 更新场景
   * @param deltaTime 帧间隔时间（秒）
   * @details 更新当前激活的场景
   */
  void Update(float deltaTime);

  /**
   * @brief 绘制场景
   * @details 绘制当前激活的场景
   */
  void Draw();

private:
  /**
   * @brief 私有构造函数
   */
  SceneManager() = default;

  std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;  ///< 场景存储
  Scene* m_activeScene = nullptr;                                     ///< 当前激活的场景
};