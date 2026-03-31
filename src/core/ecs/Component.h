#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

/**
 * @brief 组件基类
 * @details ECS 架构中的组件基类，所有组件必须继承此类
 */
class Component {
public:
    virtual ~Component() = default;
    virtual const char* GetType() const = 0;
};

/**
 * @brief 变换组件
 * @details 存储实体的位置、旋转和缩放信息
 */
class TransformComponent : public Component {
public:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};  // 欧拉角（度）
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    
    bool isDirty = true;  // 标记是否需要更新变换矩阵
    glm::mat4 modelMatrix{1.0f};
    
    const char* GetType() const override { return "TransformComponent"; }
    
    /**
     * @brief 获取变换矩阵
     * @details 根据位置、旋转、缩放计算模型矩阵
     */
    glm::mat4 GetModelMatrix();
    
    /**
     * @brief 标记变换为脏
     * @details 当变换数据改变时调用，下次获取矩阵时会重新计算
     */
    void MarkDirty() { isDirty = true; }
};

/**
 * @brief 渲染组件
 * @details 存储实体的渲染相关数据
 */
class RenderComponent : public Component {
public:
    std::string meshId;
    std::string materialId;
    bool isVisible = true;
    int renderOrder = 0;  // 渲染顺序，值越小越先渲染
    
    const char* GetType() const override { return "RenderComponent"; }
};

/**
 * @brief 名称组件
 * @details 为实体提供可读的名称
 */
class NameComponent : public Component {
public:
    std::string name;
    
    const char* GetType() const override { return "NameComponent"; }
};

/**
 * @brief 选择组件
 * @details 标记实体是否被选中
 */
class SelectionComponent : public Component {
public:
    bool isSelected = false;
    glm::vec4 selectionColor{1.0f, 0.5f, 0.0f, 1.0f};  // 选中时的颜色
    
    const char* GetType() const override { return "SelectionComponent"; }
};

// 内联实现
inline glm::mat4 TransformComponent::GetModelMatrix() {
    if (isDirty) {
        glm::mat4 pos = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0));
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
        glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
        
        modelMatrix = pos * rotZ * rotY * rotX * scl;
        isDirty = false;
    }
    return modelMatrix;
}
