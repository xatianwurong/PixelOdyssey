#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Component.h"

class World;

/**
 * @brief 实体类
 * @details ECS 架构中的实体，由一组组件组成
 * @details 实体本身不包含数据，只是组件的容器
 * @details 通过组件系统实现数据与行为的分离
 * @功能描述 作为组件的载体，提供组件管理接口
 * @设计目的 实现灵活的实体-组件系统架构
 * @使用场景 所有游戏对象和场景元素都通过实体表示
 * @关键实现 使用 type_index 作为组件类型的键
 */
class Entity
{
public:
  /**
   * @brief 默认构造函数
   * @details 创建无效实体（ID 为 0）
   */
  Entity() : m_id(0), m_world(nullptr) {}

  /**
   * @brief 构造函数
   * @param id 实体 ID
   * @param world 所属的世界指针
   */
  Entity(uint32_t id, World* world) : m_id(id), m_world(world) {}

  /**
   * @brief 拷贝构造函数
   */
  Entity(const Entity& other) = default;

  /**
   * @brief 获取实体 ID
   * @return 实体唯一标识符
   */
  uint32_t GetId() const { return m_id; }

  /**
   * @brief 检查实体是否有效
   * @return 如果实体有效返回 true
   */
  bool IsValid() const { return m_id != 0 && m_world != nullptr; }

  /**
   * @brief 添加组件
   * @tparam T 组件类型
   * @tparam Args 构造函数参数类型
   * @param args 构造函数参数
   * @return 新创建的组件引用
   */
  template<typename T, typename... Args>
  T& AddComponent(Args&&... args)
  {
    T* component = new T(std::forward<Args>(args)...);
    m_components[typeid(T)] = std::shared_ptr<Component>(component);
    return *component;
  }

  /**
   * @brief 获取组件
   * @tparam T 组件类型
   * @return 组件引用
   * @note 如果组件不存在，会自动添加该组件
   */
  template<typename T>
  T& GetComponent()
  {
    auto it = m_components.find(typeid(T));
    if (it != m_components.end())
    {
      return *std::static_pointer_cast<T>(it->second);
    }
    // 如果组件不存在，自动添加
    return AddComponent<T>();
  }

  /**
   * @brief 获取组件（const 版本）
   * @tparam T 组件类型
   * @return 组件常量引用
   */
  template<typename T>
  const T& GetComponent() const
  {
    auto it = m_components.find(typeid(T));
    if (it != m_components.end())
    {
      return *std::static_pointer_cast<T>(it->second);
    }
    static T defaultComponent;
    return defaultComponent;
  }

  /**
   * @brief 检查是否有组件
   * @tparam T 组件类型
   * @return 如果有该组件返回 true
   */
  template<typename T>
  bool HasComponent() const
  {
    return m_components.find(typeid(T)) != m_components.end();
  }

  /**
   * @brief 移除组件
   * @tparam T 组件类型
   */
  template<typename T>
  void RemoveComponent()
  {
    m_components.erase(typeid(T));
  }

  /**
   * @brief 获取实体名称
   * @return 实体名称（如果有 NameComponent）或默认名称
   */
  std::string GetName() const
  {
    if (HasComponent<NameComponent>())
    {
      return GetComponent<NameComponent>().name;
    }
    return "Entity_" + std::to_string(m_id);
  }

  /**
   * @brief 等于运算符重载
   * @param other 另一个实体
   * @return 如果 ID 和世界相同返回 true
   */
  bool operator==(const Entity& other) const
  {
    return m_id == other.m_id && m_world == other.m_world;
  }

  /**
   * @brief 不等于运算符重载
   * @param other 另一个实体
   * @return 如果不相等返回 true
   */
  bool operator!=(const Entity& other) const
  {
    return !(*this == other);
  }

private:
  uint32_t m_id;                                              ///< 实体唯一 ID
  World* m_world;                                             ///< 所属世界指针
  std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;  ///< 组件映射表

  friend class World;  ///< 允许 World 类访问私有成员
};

/**
 * @brief Entity 的哈希函数特化
 * @details 为 Entity 提供哈希支持，用于 unordered_map
 */
namespace std
{
  template<>
  struct hash<Entity>
  {
    size_t operator()(const Entity& e) const
    {
      return std::hash<uint32_t>()(e.GetId());
    }
  };
}
