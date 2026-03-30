#include "SceneManager.h"
#include <iostream>

SceneManager& SceneManager::Instance()
{
  static SceneManager instance;
  return instance;
}

void SceneManager::AddScene(Scene* scene)
{
  if (scene)
  {
    m_scenes[scene->GetName()] = std::unique_ptr<Scene>(scene);

    if (!m_activeScene)
    {
      m_activeScene = scene;
    }
  }
}

void SceneManager::RemoveScene(const std::string& name)
{
  auto it = m_scenes.find(name);
  if (it != m_scenes.end())
  {
    if (it->second.get() == m_activeScene)
    {
      m_activeScene = nullptr;
    }
    m_scenes.erase(it);
  }
}

Scene* SceneManager::GetScene(const std::string& name)
{
  auto it = m_scenes.find(name);
  return it != m_scenes.end() ? it->second.get() : nullptr;
}

void SceneManager::SetActiveScene(const std::string& name)
{
  m_activeScene = GetScene(name);
}

Scene* SceneManager::GetActiveScene()
{
  return m_activeScene;
}

void SceneManager::Update(float deltaTime)
{
  if (m_activeScene)
  {
    m_activeScene->Update(deltaTime);
  }
}

void SceneManager::Draw()
{
  if (m_activeScene)
  {
    m_activeScene->Draw();
  }
}