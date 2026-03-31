#pragma once
#include "Command.h"
#include "../ecs/World.h"
#include "../ecs/Component.h"
#include <glm/glm.hpp>

/**
 * @brief 创建实体命令
 * @details 创建一个新实体，支持撤销
 */
class CreateEntityCommand : public Command {
public:
    CreateEntityCommand(const std::string& name = "New Entity",
                       const glm::vec3& position = glm::vec3(0, 0, 0))
        : m_name(name), m_position(position), m_createdEntityId(0) {}
    
    void Execute() override {
        auto& world = World::Instance();
        Entity entity = world.CreateEntity(m_name);
        
        // 添加变换组件
        auto& transform = entity.AddComponent<TransformComponent>();
        transform.position = m_position;
        
        m_createdEntityId = entity.GetId();
    }
    
    void Undo() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_createdEntityId) {
                world.DestroyEntity(entity);
                break;
            }
        }
    }
    
    std::string GetName() const override {
        return "CreateEntity: " + m_name;
    }

private:
    std::string m_name;
    glm::vec3 m_position;
    uint32_t m_createdEntityId;
};

/**
 * @brief 删除实体命令
 * @details 删除一个实体，支持撤销
 */
class DeleteEntityCommand : public Command {
public:
    DeleteEntityCommand(uint32_t entityId) : m_entityId(entityId) {}
    
    void Execute() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_entityId) {
                // 保存实体数据以便撤销
                if (entity.HasComponent<NameComponent>()) {
                    m_name = entity.GetComponent<NameComponent>().name;
                }
                if (entity.HasComponent<TransformComponent>()) {
                    m_position = entity.GetComponent<TransformComponent>().position;
                }
                
                world.DestroyEntity(entity);
                break;
            }
        }
    }
    
    void Undo() override {
        auto& world = World::Instance();
        Entity entity = world.CreateEntity(m_name);
        
        auto& transform = entity.AddComponent<TransformComponent>();
        transform.position = m_position;
    }
    
    std::string GetName() const override {
        return "DeleteEntity";
    }

private:
    uint32_t m_entityId;
    std::string m_name;
    glm::vec3 m_position;
};

/**
 * @brief 移动实体命令
 * @details 移动实体到新位置，支持撤销/重做
 */
class MoveEntityCommand : public Command {
public:
    MoveEntityCommand(uint32_t entityId, const glm::vec3& newPosition)
        : m_entityId(entityId), m_newPosition(newPosition) {}
    
    void Execute() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_entityId) {
                if (!m_hasOldPosition) {
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
    
    void Undo() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_entityId) {
                auto& transform = entity.GetComponent<TransformComponent>();
                transform.position = m_oldPosition;
                transform.MarkDirty();
                break;
            }
        }
    }
    
    std::string GetName() const override {
        return "MoveEntity";
    }
    
    bool CanMerge(const Command* other) const override {
        if (other->GetName() != GetName()) {
            return false;
        }
        const auto* moveCmd = dynamic_cast<const MoveEntityCommand*>(other);
        return moveCmd && moveCmd->m_entityId == m_entityId;
    }
    
    bool Merge(Command* other) override {
        auto* moveCmd = dynamic_cast<MoveEntityCommand*>(other);
        if (moveCmd && moveCmd->m_entityId == m_entityId) {
            m_newPosition = moveCmd->m_newPosition;
            return true;
        }
        return false;
    }

private:
    uint32_t m_entityId;
    glm::vec3 m_oldPosition;
    glm::vec3 m_newPosition;
    bool m_hasOldPosition = false;
};
