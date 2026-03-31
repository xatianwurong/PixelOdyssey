#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include <glm/glm.hpp>

/**
 * @brief 缩放命令
 */
class ScaleEntityCommand : public Command {
public:
    ScaleEntityCommand(uint32_t entityId, const glm::vec3& newScale)
        : m_entityId(entityId), m_newScale(newScale) {}
    
    void Execute() override {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (entity.GetId() == m_entityId) {
                if (!m_hasOldScale) {
                    m_oldScale = entity.GetComponent<TransformComponent>().scale;
                    m_hasOldScale = true;
                }
                
                auto& transform = entity.GetComponent<TransformComponent>();
                transform.scale = m_newScale;
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
                transform.scale = m_oldScale;
                transform.MarkDirty();
                break;
            }
        }
    }
    
    std::string GetName() const override {
        return "ScaleEntity";
    }

private:
    uint32_t m_entityId;
    glm::vec3 m_oldScale;
    glm::vec3 m_newScale;
    bool m_hasOldScale = false;
};

/**
 * @brief 缩放工具
 * @details 用于缩放选中的实体
 */
class ScaleTool : public Tool {
public:
    ScaleTool() 
        : m_isDragging(false)
        , m_selectedEntityId(0)
        , m_scaleMode(2) // 2=均匀缩放，0=X 轴，1=Y 轴，2=Z 轴
        , m_dragStartX(0)
        , m_dragStartY(0) {}
    
    ToolType GetType() const override {
        return ToolType::Scale;
    }
    
    std::string GetName() const override {
        return "Scale Tool";
    }
    
    void OnActivate() override {
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
        SUBSCRIBE_EVENT(MouseScrolledEvent, OnMouseScrolled);
    }
    
    void OnDeactivate() override {
        m_isDragging = false;
        m_selectedEntityId = 0;
    }
    
    void Render() override {
        if (m_selectedEntityId != 0) {
            // TODO: 渲染缩放 Gizmo（三个轴向的方块）
        }
    }
    
    /**
     * @brief 设置缩放模式
     * @param mode 0=X 轴，1=Y 轴，2=Z 轴，3= 均匀
     */
    void SetScaleMode(int mode) {
        m_scaleMode = mode;
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
                            m_originalScale = entity.GetComponent<TransformComponent>().scale;
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
                auto cmd = std::make_unique<ScaleEntityCommand>(
                    m_selectedEntityId, m_currentScale);
                CommandStack::Instance().Execute(std::move(cmd));
            }
            
            m_selectedEntityId = 0;
            event.SetHandled(true);
        }
    }
    
    void OnMouseMoved(MouseMovedEvent& event) {
        if (m_isDragging && m_selectedEntityId != 0) {
            float deltaY = event.GetY() - m_dragStartY;
            
            // 缩放系数
            float scaleDelta = 1.0f + deltaY * 0.01f;
            
            m_currentScale = m_originalScale;
            
            switch (m_scaleMode) {
                case 0: // X 轴
                    m_currentScale.x *= scaleDelta;
                    break;
                case 1: // Y 轴
                    m_currentScale.y *= scaleDelta;
                    break;
                case 2: // Z 轴
                    m_currentScale.z *= scaleDelta;
                    break;
                case 3: // 均匀缩放
                    m_currentScale *= scaleDelta;
                    break;
            }
            
            // 确保缩放值为正
            m_currentScale.x = max(0.01f, m_currentScale.x);
            m_currentScale.y = max(0.01f, m_currentScale.y);
            m_currentScale.z = max(0.01f, m_currentScale.z);
            
            // 实时更新
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_selectedEntityId) {
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        transform.scale = m_currentScale;
                        transform.MarkDirty();
                    }
                    break;
                }
            }
            
            event.SetHandled(true);
        }
    }
    
    void OnMouseScrolled(MouseScrolledEvent& event) {
        // 滚轮缩放（当没有拖动时）
        if (!m_isDragging && m_selectedEntityId != 0) {
            float scrollDelta = event.GetYOffset();
            float scaleDelta = 1.0f + scrollDelta * 0.1f;
            
            m_currentScale = m_originalScale * scaleDelta;
            
            // 实时更新
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_selectedEntityId) {
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        transform.scale = m_currentScale;
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
    int m_scaleMode;
    float m_dragStartX, m_dragStartY;
    glm::vec3 m_originalScale;
    glm::vec3 m_currentScale;
};
