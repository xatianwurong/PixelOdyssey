#pragma once
#include "Core/Entity/Object.h"
#include <unordered_map>
#include <memory>

/**
 * @class RenderManager
 * @brief 渲染管理器类
 * @details 管理所有实体对象的生命周期和渲染
 * @功能描述 提供实体的添加、移除和绘制功能
 * @设计目的 使用单例模式，确保全局只有一个渲染管理器实例
 * @使用场景 用于统一管理场景中的所有可渲染对象
 * @关键实现 使用单例模式，通过Instance()方法获取实例
 */
class RenderManager
{
public:
  static RenderManager& Instance();
  ~RenderManager();

  void AddObject(Object* object);
  void RemoveObject(Object* object);
  void Draw();

private:
  RenderManager();

  std::unordered_map<std::string, std::unique_ptr<Object>> m_objects;
};