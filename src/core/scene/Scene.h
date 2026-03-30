#pragma once
#include "../entity/Object.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

/**
 * @class Scene
 * @brief 场景类
 * @details 管理场景中的对象，包括对象的添加、移除、更新和绘制
 * @功能描述 提供场景中对象的管理和操作功能
 * @设计目的 为场景管理提供统一的接口
 * @使用场景 用于管理游戏或应用中的不同场景
 * @关键实现 包含对象的存储、更新顺序和绘制顺序
 */
class Scene
{
public:
  Scene(const std::string& name);
  ~Scene() = default;

  void AddObject(Object* object);
  void RemoveObject(const std::string& id);
  Object* GetObject(const std::string& id);
  void Clear();

  const std::string& GetName() const { return m_name; }

  void Update(float deltaTime);
  void Draw();

private:
  std::string m_name;
  std::unordered_map<std::string, std::unique_ptr<Object>> m_objects;
  std::vector<Object*> m_updateOrder;
  std::vector<Object*> m_drawOrder;
};