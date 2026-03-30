#include "Scene.h"
#include <iostream>

Scene::Scene(const std::string& name)
  : m_name(name)
{}

void Scene::AddObject(Object* object)
{
  if (object)
  {
    m_objects[object->GetId()] = std::unique_ptr<Object>(object);
    m_updateOrder.push_back(object);
    m_drawOrder.push_back(object);
  }
}

void Scene::RemoveObject(const std::string& id)
{
  auto it = m_objects.find(id);
  if (it != m_objects.end())
  {
    Object* obj = it->second.get();

    auto updateIt = std::find(m_updateOrder.begin(), m_updateOrder.end(), obj);
    if (updateIt != m_updateOrder.end())
    {
      m_updateOrder.erase(updateIt);
    }

    auto drawIt = std::find(m_drawOrder.begin(), m_drawOrder.end(), obj);
    if (drawIt != m_drawOrder.end())
    {
      m_drawOrder.erase(drawIt);
    }

    m_objects.erase(it);
  }
}

Object* Scene::GetObject(const std::string& id)
{
  auto it = m_objects.find(id);
  return it != m_objects.end() ? it->second.get() : nullptr;
}

void Scene::Clear()
{
  m_objects.clear();
  m_updateOrder.clear();
  m_drawOrder.clear();
}

void Scene::Update(float deltaTime)
{
  for (auto* object : m_updateOrder)
  {
    if (object)
    {
      object->Update(deltaTime);
    }
  }
}

void Scene::Draw()
{
  for (auto* object : m_drawOrder)
  {
    if (object)
    {
      object->Draw();
    }
  }
}