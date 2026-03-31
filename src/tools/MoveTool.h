#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include "../core/command/CreateEntityCommand.h"
#include <glm/glm.hpp>

/**
 * @brief 移动工具
 * @details 用于移动选中的实体
 */
class MoveTool : public Tool {
public:
    MoveTool() 
        : m_isDragging(false)
        , m_selectedEntityId(0)
        , m_dragStartX(0)
        , m_dragStartY(0) {}
    
    ToolType GetType() const override {
        return ToolType::Move;
    }
    
    std::string GetName() const override {
        return "Move Tool";
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
            // TODO: 渲染移动 Gizmo（三轴箭头）
        }
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
                        
                        // 保存原始位置用于命令
                        if (entity.HasComponent<TransformComponent>()) {
                            m_originalPosition = entity.GetComponent<TransformComponent>().position;
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
                // 执行移动命令
                auto cmd = std::make_unique<MoveEntityCommand>(
                    m_selectedEntityId, m_currentPosition);
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
            
            // 更新位置（需要转换为世界坐标）
            m_currentPosition = m_originalPosition + glm::vec3(deltaX * 0.01f, deltaY * 0.01f, 0);
            
            // 实时更新实体位置
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_selectedEntityId) {
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        transform.position = m_currentPosition;
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
    float m_dragStartX, m_dragStartY;
    glm::vec3 m_originalPosition;
    glm::vec3 m_currentPosition;
};
