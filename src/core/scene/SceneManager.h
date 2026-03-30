#pragma once
#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>

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