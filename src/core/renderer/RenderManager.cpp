#include "RenderManager.h"

RenderManager& RenderManager::Instance()
{
  static RenderManager instance;
  return instance;
}

RenderManager::RenderManager()
{}

RenderManager::~RenderManager()
{}

void RenderManager::AddObject(Object* object)
{
  if (object)
  {
    m_objects[object->GetId()] = std::unique_ptr<Object>(object);
  }
}

void RenderManager::RemoveObject(Object* object)
{
  if (object)
  {
    m_objects.erase(object->GetId());
  }
}

void RenderManager::Draw()
{
  for (const auto& pair : m_objects)
  {
    pair.second->Draw();
  }
}
