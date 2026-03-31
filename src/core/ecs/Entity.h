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
 */
class Entity {
public:
    /**
     * @brief 默认构造函数
     */
    Entity() : m_id(0), m_world(nullptr) {}
    
    /**
     * @brief 构造函数
     * @param id 实体 ID
     * @param world 所属的世界
     */
    Entity(uint32_t id, World* world) : m_id(id), m_world(world) {}
    
    /**
     * @brief 拷贝构造函数
     */
    Entity(const Entity& other) = default;
    
    /**
     * @brief 获取实体 ID
     * @return 实体 ID
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
     * @return 组件引用
     */
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        T* component = new T(std::forward<Args>(args)...);
        m_components[typeid(T)] = std::shared_ptr<Component>(component);
        return *component;
    }
    
    /**
     * @brief 获取组件
     * @tparam T 组件类型
     * @return 组件引用
     */
    template<typename T>
    T& GetComponent() {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {
            return *std::static_pointer_cast<T>(it->second);
        }
        // 如果组件不存在，自动添加
        return AddComponent<T>();
    }
    
    /**
     * @brief 获取组件（const 版本）
     * @tparam T 组件类型
     * @return 组件引用
     */
    template<typename T>
    const T& GetComponent() const {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {
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
    bool HasComponent() const {
        return m_components.find(typeid(T)) != m_components.end();
    }
    
    /**
     * @brief 移除组件
     * @tparam T 组件类型
     */
    template<typename T>
    void RemoveComponent() {
        m_components.erase(typeid(T));
    }
    
    /**
     * @brief 获取实体名称
     * @return 实体名称
     */
    std::string GetName() const {
        if (HasComponent<NameComponent>()) {
            return GetComponent<NameComponent>().name;
        }
        return "Entity_" + std::to_string(m_id);
    }
    
    /**
     * @brief 等于运算符
     */
    bool operator==(const Entity& other) const {
        return m_id == other.m_id && m_world == other.m_world;
    }
    
    /**
     * @brief 不等于运算符
     */
    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

private:
    uint32_t m_id;
    World* m_world;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
    
    friend class World;
};

// 为 Entity 提供哈希函数，用于 unordered_map
namespace std {
    template<>
    struct hash<Entity> {
        size_t operator()(const Entity& e) const {
            return std::hash<uint32_t>()(e.GetId());
        }
    };
}
