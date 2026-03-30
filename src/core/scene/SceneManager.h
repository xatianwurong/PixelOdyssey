#pragma once
#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>

/**
 * @class SceneManager
 * @brief 场景管理器类
 * @details 管理多个场景的切换和操作
 * @功能描述 提供场景的添加、移除、获取和切换功能
 * @设计目的 使用单例模式，确保场景的统一管理
 * @使用场景 用于管理游戏或应用中的多个场景
 * @关键实现 使用单例模式，通过Instance()方法获取实例
 */
class SceneManager
{
public:
    static SceneManager& Instance();
    ~SceneManager() = default;

    void AddScene(Scene* scene);
    void RemoveScene(const std::string& name);
    Scene* GetScene(const std::string& name);
    void SetActiveScene(const std::string& name);
    Scene* GetActiveScene();

    void Update(float deltaTime);
    void Draw();

private:
    SceneManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
    Scene* m_activeScene = nullptr;
};