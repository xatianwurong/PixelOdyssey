#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include "../core/command/CreateEntityCommand.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 选择工具
 * @details 用于选择和操作场景中的实体
 */
class SelectTool : public Tool {
public:
    SelectTool() : m_hoveredEntityId(0) {}
    
    ToolType GetType() const override {
        return ToolType::Select;
    }
    
    std::string GetName() const override {
        return "Select Tool";
    }
    
    void OnActivate() override {
        // 订阅鼠标事件
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
    }
    
    void OnDeactivate() override {
        m_hoveredEntityId = 0;
    }
    
    bool HandleEvent(Event& event) override {
        // 由订阅的回调处理
        return event.IsHandled();
    }
    
    void Render() override {
        // 渲染悬停高亮
        if (m_hoveredEntityId != 0) {
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_hoveredEntityId && entity.HasComponent<RenderComponent>()) {
                    // TODO: 渲染高亮轮廓
                    break;
                }
            }
        }
    }

private:
    void OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        if (event.GetButton() == 0) { // 左键
            float x = event.GetX();
            float y = event.GetY();
            
            // TODO: 实现射线投射选择逻辑
            // 这里需要根据鼠标位置进行射线投射，检测与实体的交点
            
            // 示例：清除之前的选择
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.HasComponent<SelectionComponent>()) {
                    auto& selection = entity.GetComponent<SelectionComponent>();
                    if (selection.isSelected) {
                        selection.isSelected = false;
                        // 发布选择改变事件
                        EventBus::Instance().Publish<EntitySelectionChangedEvent>(
                            entity.GetId(), false);
                    }
                }
            }
            
            // TODO: 选择新实体
            // 这里需要根据射线投射结果选择实体
            
            event.SetHandled(true);
        }
    }
    
    void OnMouseMoved(MouseMovedEvent& event) {
        float x = event.GetX();
        float y = event.GetY();
        
        // TODO: 更新悬停实体
        // 这里需要根据鼠标位置更新悬停的实体
        
        event.SetHandled(false); // 不阻塞其他处理
    }
    
    uint32_t m_hoveredEntityId;
};
