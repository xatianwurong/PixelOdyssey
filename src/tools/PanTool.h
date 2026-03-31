#pragma once
#include "Tool.h"
#include "../core/event/EventBus.h"
#include <glm/glm.hpp>

/**
 * @brief 平移工具
 * @details 用于平移相机视图
 */
class PanTool : public Tool {
public:
    PanTool() 
        : m_isDragging(false)
        , m_lastX(0)
        , m_lastY(0) {}
    
    ToolType GetType() const override {
        return ToolType::Pan;
    }
    
    std::string GetName() const override {
        return "Pan Tool";
    }
    
    void OnActivate() override {
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
    }
    
    void OnDeactivate() override {
        m_isDragging = false;
    }

private:
    void OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        if (event.GetButton() == 1) { // 中键
            m_isDragging = true;
            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }
    }
    
    void OnMouseButtonReleased(MouseButtonReleasedEvent& event) {
        if (event.GetButton() == 1) {
            m_isDragging = false;
            event.SetHandled(true);
        }
    }
    
    void OnMouseMoved(MouseMovedEvent& event) {
        if (m_isDragging) {
            float deltaX = event.GetX() - m_lastX;
            float deltaY = event.GetY() - m_lastY;
            
            // TODO: 更新相机位置
            // Camera::Instance().Translate(-deltaX * 0.01f, deltaY * 0.01f, 0);
            
            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }
    }
    
    bool m_isDragging;
    float m_lastX, m_lastY;
};
