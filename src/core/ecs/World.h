#pragma once
#include "Entity.h"
#include <vector>
#include <algorithm>
#include <functional>

/**
 * @brief 世界类
 * @details ECS 架构的世界，管理所有实体
 * @details 提供实体的创建、销毁和查询功能
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 管理所有实体的生命周期和查询
 * @设计目的 提供统一的实体管理平台
 * @使用场景 所有实体的创建、销毁和查询都通过此类进行
 * @关键实现 单例模式，禁止拷贝和赋值
 */
class World
{
public:
  /**
   * @brief 获取世界单例实例
   * @return 世界实例引用
   */
  static World& Instance()
  {
    static World instance;
    return instance;
  }

  /**
   * @brief 创建实体
   * @param name 实体名称（可选，为空则不添加名称组件）
   * @return 新创建的实体对象
   */
  Entity CreateEntity(const std::string& name = "")
  {
    Entity entity(++m_nextEntityId, this);
    m_entities.push_back(entity);

    if (!name.empty())
    {
      entity.AddComponent<NameComponent>().name = name;
    }

    return entity;
  }

  /**
   * @brief 销毁实体
   * @param entity 要销毁的实体对象
   */
  void DestroyEntity(Entity entity)
  {
    m_entities.erase(
      std::remove(m_entities.begin(), m_entities.end(), entity),
      m_entities.end()
    );
  }

  /**
   * @brief 获取所有实体
   * @return 实体列表的常量引用
   */
  const std::vector<Entity>& GetEntities() const
  {
    return m_entities;
  }

  /**
   * @brief 根据条件查询实体
   * @tparam Predicate 谓词函数类型
   * @param predicate 查询条件函数
   * @return 满足条件的实体列表
   */
  template<typename Predicate>
  std::vector<Entity> FindEntities(Predicate predicate)
  {
    std::vector<Entity> result;
    for (const auto& entity : m_entities)
    {
      if (predicate(entity))
      {
        result.push_back(entity);
      }
    }
    return result;
  }

  /**
   * @brief 根据组件类型查询实体
   * @tparam T 组件类型
   * @return 拥有指定组件类型的实体列表
   */
  template<typename T>
  std::vector<Entity> GetEntitiesWithComponent()
  {
    std::vector<Entity> result;
    for (const auto& entity : m_entities)
    {
      if (entity.HasComponent<T>())
      {
        result.push_back(entity);
      }
    }
    return result;
  }

  /**
   * @brief 获取实体数量
   * @return 当前世界中的实体总数
   */
  size_t GetEntityCount() const
  {
    return m_entities.size();
  }

  /**
   * @brief 清空所有实体
   * @details 删除所有实体并重置实体 ID 计数器
   */
  void Clear()
  {
    m_entities.clear();
    m_nextEntityId = 0;
  }

private:
  /**
   * @brief 私有构造函数
   */
  World() = default;

  /**
   * @brief 私有析构函数
   */
  ~World() = default;

  /**
   * @brief 禁用拷贝构造函数
   */
  World(const World&) = delete;

  /**
   * @brief 禁用赋值运算符
   */
  World& operator=(const World&) = delete;

  std::vector<Entity> m_entities;       ///< 实体列表
  uint32_t m_nextEntityId = 0;          ///< 下一个实体的 ID

  friend class Entity;  ///< 允许 Entity 类访问私有成员
};
