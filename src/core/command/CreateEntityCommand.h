#pragma once
#include "Command.h"
#include "../ecs/World.h"
#include "../ecs/Component.h"
#include <glm/glm.hpp>

/**
 * @brief 创建实体命令
 * @details 创建一个新实体，支持撤销操作
 * @details 实现命令模式，可通过 CommandStack 管理
 * @功能描述 封装创建实体的操作
 * @设计目的 支持创建操作的撤销/重做
 * @使用场景 编辑器中创建新实体
 * @关键实现 记录创建的实体 ID，撤销时销毁该实体
 */
class CreateEntityCommand : public Command
{
public:
  /**
   * @brief 构造函数
   * @param name 实体名称（默认"New Entity"）
   * @param position 实体初始位置（默认原点）
   */
  CreateEntityCommand(const std::string& name = "New Entity",
                     const glm::vec3& position = glm::vec3(0, 0, 0))
    : m_name(name), m_position(position), m_createdEntityId(0) {}

  /**
   * @brief 执行命令
   * @details 创建实体并添加变换组件
   */
  void Execute() override
  {
    auto& world = World::Instance();
    Entity entity = world.CreateEntity(m_name);

    // 添加变换组件
    auto& transform = entity.AddComponent<TransformComponent>();
    transform.position = m_position;

    m_createdEntityId = entity.GetId();
  }

  /**
   * @brief 撤销命令
   * @details 销毁创建的实体
   */
  void Undo() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_createdEntityId)
      {
        world.DestroyEntity(entity);
        break;
      }
    }
  }

  /**
   * @brief 获取命令名称
   * @return 命令名称字符串
   */
  std::string GetName() const override
  {
    return "CreateEntity: " + m_name;
  }

private:
  std::string m_name;              ///< 实体名称
  glm::vec3 m_position;            ///< 实体位置
  uint32_t m_createdEntityId;      ///< 创建的实体 ID
};

/**
 * @brief 删除实体命令
 * @details 删除一个实体，支持撤销操作
 * @details 保存实体数据以便撤销时恢复
 * @功能描述 封装删除实体的操作
 * @设计目的 支持删除操作的撤销/重做
 * @使用场景 编辑器中删除实体
 * @关键实现 保存实体名称和位置，撤销时重新创建
 */
class DeleteEntityCommand : public Command
{
public:
  /**
   * @brief 构造函数
   * @param entityId 要删除的实体 ID
   */
  DeleteEntityCommand(uint32_t entityId) : m_entityId(entityId) {}

  /**
   * @brief 执行命令
   * @details 删除实体并保存实体数据
   */
  void Execute() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        // 保存实体数据以便撤销
        if (entity.HasComponent<NameComponent>())
        {
          m_name = entity.GetComponent<NameComponent>().name;
        }
        if (entity.HasComponent<TransformComponent>())
        {
          m_position = entity.GetComponent<TransformComponent>().position;
        }

        world.DestroyEntity(entity);
        break;
      }
    }
  }

  /**
   * @brief 撤销命令
   * @details 重新创建实体
   */
  void Undo() override
  {
    auto& world = World::Instance();
    Entity entity = world.CreateEntity(m_name);

    auto& transform = entity.AddComponent<TransformComponent>();
    transform.position = m_position;
  }

  /**
   * @brief 获取命令名称
   * @return 命令名称字符串
   */
  std::string GetName() const override
  {
    return "DeleteEntity";
  }

private:
  uint32_t m_entityId;      ///< 实体 ID
  std::string m_name;       ///< 实体名称（保存用于撤销）
  glm::vec3 m_position;     ///< 实体位置（保存用于撤销）
};

/**
 * @brief 移动实体命令
 * @details 移动实体到新位置，支持撤销/重做
 * @details 支持命令合并（连续移动可合并为一个命令）
 * @功能描述 封装移动实体的操作
 * @设计目的 支持移动操作的撤销/重做和命令合并
 * @使用场景 编辑器中移动实体位置
 * @关键实现 记录原始位置和新位置，支持命令合并优化
 */
class MoveEntityCommand : public Command
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   * @param newPosition 新位置
   */
  MoveEntityCommand(uint32_t entityId, const glm::vec3& newPosition)
    : m_entityId(entityId), m_newPosition(newPosition) {}

  /**
   * @brief 执行命令
   * @details 移动实体到新位置
   */
  void Execute() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        if (!m_hasOldPosition)
        {
          m_oldPosition = entity.GetComponent<TransformComponent>().position;
          m_hasOldPosition = true;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position = m_newPosition;
        transform.MarkDirty();
        break;
      }
    }
  }

  /**
   * @brief 撤销命令
   * @details 恢复实体到原始位置
   */
  void Undo() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position = m_oldPosition;
        transform.MarkDirty();
        break;
      }
    }
  }

  /**
   * @brief 获取命令名称
   * @return 命令名称字符串
   */
  std::string GetName() const override
  {
    return "MoveEntity";
  }

  /**
   * @brief 检查是否可以合并命令
   * @param other 另一个命令指针
   * @return 如果同一实体的移动命令返回 true
   */
  bool CanMerge(const Command* other) const override
  {
    if (other->GetName() != GetName())
    {
      return false;
    }
    const auto* moveCmd = dynamic_cast<const MoveEntityCommand*>(other);
    return moveCmd && moveCmd->m_entityId == m_entityId;
  }

  /**
   * @brief 合并命令
   * @param other 另一个命令指针
   * @return 是否合并成功
   */
  bool Merge(Command* other) override
  {
    auto* moveCmd = dynamic_cast<MoveEntityCommand*>(other);
    if (moveCmd && moveCmd->m_entityId == m_entityId)
    {
      m_newPosition = moveCmd->m_newPosition;
      return true;
    }
    return false;
  }

private:
  uint32_t m_entityId;           ///< 实体 ID
  glm::vec3 m_oldPosition;       ///< 原始位置
  glm::vec3 m_newPosition;       ///< 新位置
  bool m_hasOldPosition = false; ///< 是否已记录原始位置
};
