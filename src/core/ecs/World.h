#pragma once
#include "Entity.h"
#include <vector>
#include <algorithm>
#include <functional>

/**
 * @brief 世界类
 * @details ECS 架构的世界，管理所有实体
 * @details 提供实体的创建、销毁和查询功能
 */
class World {
public:
    /**
     * @brief 获取单例实例
     * @return 世界实例引用
     */
    static World& Instance() {
        static World instance;
        return instance;
    }
    
    /**
     * @brief 创建实体
     * @param name 实体名称（可选）
     * @return 新创建的实体
     */
    Entity CreateEntity(const std::string& name = "") {
        Entity entity(++m_nextEntityId, this);
        m_entities.push_back(entity);
        
        if (!name.empty()) {
            entity.AddComponent<NameComponent>().name = name;
        }
        
        return entity;
    }
    
    /**
     * @brief 销毁实体
     * @param entity 要销毁的实体
     */
    void DestroyEntity(Entity entity) {
        m_entities.erase(
            std::remove(m_entities.begin(), m_entities.end(), entity),
            m_entities.end()
        );
    }
    
    /**
     * @brief 获取所有实体
     * @return 实体列表引用
     */
    const std::vector<Entity>& GetEntities() const {
        return m_entities;
    }
    
    /**
     * @brief 根据条件查询实体
     * @tparam Predicate 谓词函数类型
     * @param predicate 查询条件
     * @return 满足条件的实体列表
     */
    template<typename Predicate>
    std::vector<Entity> FindEntities(Predicate predicate) {
        std::vector<Entity> result;
        for (const auto& entity : m_entities) {
            if (predicate(entity)) {
                result.push_back(entity);
            }
        }
        return result;
    }
    
    /**
     * @brief 根据组件类型查询实体
     * @tparam T 组件类型
     * @return 拥有该组件的实体列表
     */
    template<typename T>
    std::vector<Entity> GetEntitiesWithComponent() {
        std::vector<Entity> result;
        for (const auto& entity : m_entities) {
            if (entity.HasComponent<T>()) {
                result.push_back(entity);
            }
        }
        return result;
    }
    
    /**
     * @brief 获取实体数量
     * @return 实体数量
     */
    size_t GetEntityCount() const {
        return m_entities.size();
    }
    
    /**
     * @brief 清空所有实体
     */
    void Clear() {
        m_entities.clear();
        m_nextEntityId = 0;
    }

private:
    World() = default;
    ~World() = default;
    
    // 禁止拷贝
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    
    std::vector<Entity> m_entities;
    uint32_t m_nextEntityId = 0;
    
    friend class Entity;
};
