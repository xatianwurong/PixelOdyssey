#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 旋转命令
 */
class RotateEntityCommand : public Command {
public:
    RotateEntityCommand(uint32_t entityId, const glm::vec3& newRotation)
        : m_entityId(entityId), m_newRotation(newRotation) {}
    
    void Execute() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_entityId) {
                if (!m_hasOldRotation) {
                    m_oldRotation = entity.GetComponent<TransformComponent>().rotation;
                    m_hasOldRotation = true;
                }
                
                auto& transform = entity.GetComponent<TransformComponent>();
                transform.rotation = m_newRotation;
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
                transform.rotation = m_oldRotation;
                transform.MarkDirty();
                break;
            }
        }
    }
    
    std::string GetName() const override {
        return "RotateEntity";
    }

private:
    uint32_t m_entityId;
    glm::vec3 m_oldRotation;
    glm::vec3 m_newRotation;
    bool m_hasOldRotation = false;
};

/**
 * @brief 旋转工具
 * @details 用于旋转选中的实体
 */
class RotateTool : public Tool {
public:
    RotateTool() 
        : m_isDragging(false)
        , m_selectedEntityId(0)
        , m_rotationMode(0) // 0=X, 1=Y, 2=Z
        , m_dragStartX(0)
        , m_dragStartY(0) {}
    
    ToolType GetType() const override {
        return ToolType::Rotate;
    }
    
    std::string GetName() const override {
        return "Rotate Tool";
    }
    
    void OnActivate() override {
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
    }
    
    void OnDeactivate() override {
        m_isDragging = false;
        m_selectedEntityId = 0;
    }
    
    void Render() override {
        if (m_selectedEntityId != 0) {
            // TODO: 渲染旋转 Gizmo（三个圆环）
        }
    }
    
    /**
     * @brief 设置旋转模式
     * @param mode 0=X 轴，1=Y 轴，2=Z 轴
     */
    void SetRotationMode(int mode) {
        m_rotationMode = mode;
    }

private:
    void OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        if (event.GetButton() == 0) { // 左键
            m_isDragging = true;
            m_dragStartX = event.GetX();
            m_dragStartY = event.GetY();
            
            // 获取选中的实体
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.HasComponent<SelectionComponent>()) {
                    auto& selection = entity.GetComponent<SelectionComponent>();
                    if (selection.isSelected) {
                        m_selectedEntityId = entity.GetId();
                        
                        if (entity.HasComponent<TransformComponent>()) {
                            m_originalRotation = entity.GetComponent<TransformComponent>().rotation;
                        }
                        break;
                    }
                }
            }
            
            event.SetHandled(true);
        }
    }
    
    void OnMouseButtonReleased(MouseButtonReleasedEvent& event) {
        if (event.GetButton() == 0 && m_isDragging) {
            m_isDragging = false;
            
            if (m_selectedEntityId != 0) {
                auto cmd = std::make_unique<RotateEntityCommand>(
                    m_selectedEntityId, m_currentRotation);
                CommandStack::Instance().Execute(std::move(cmd));
            }
            
            m_selectedEntityId = 0;
            event.SetHandled(true);
        }
    }
    
    void OnMouseMoved(MouseMovedEvent& event) {
        if (m_isDragging && m_selectedEntityId != 0) {
            float deltaX = event.GetX() - m_dragStartX;
            float deltaY = event.GetY() - m_dragStartY;
            
            // 根据旋转模式更新角度
            float angleDelta = 0.5f; // 每像素 0.5 度
            
            m_currentRotation = m_originalRotation;
            
            switch (m_rotationMode) {
                case 0: // X 轴
                    m_currentRotation.x += deltaY * angleDelta;
                    break;
                case 1: // Y 轴
                    m_currentRotation.y += deltaX * angleDelta;
                    break;
                case 2: // Z 轴
                    m_currentRotation.z += deltaX * angleDelta;
                    break;
            }
            
            // 实时更新
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_selectedEntityId) {
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        transform.rotation = m_currentRotation;
                        transform.MarkDirty();
                    }
                    break;
                }
            }
            
            event.SetHandled(true);
        }
    }
    
    bool m_isDragging;
    uint32_t m_selectedEntityId;
    int m_rotationMode;
    float m_dragStartX, m_dragStartY;
    glm::vec3 m_originalRotation;
    glm::vec3 m_currentRotation;
};
