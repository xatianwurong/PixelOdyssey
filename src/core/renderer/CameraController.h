#pragma once
#include "Camera.h"
#include "../event/Event.h"
#include "../event/EventBus.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 相机控制器
 * @details 处理相机的用户输入控制（平移/旋转/缩放）
 */
class CameraController {
public:
    CameraController(Camera* camera) 
        : m_camera(camera)
        , m_isPanning(false)
        , m_isOrbiting(false)
        , m_isZooming(false)
        , m_lastX(0)
        , m_lastY(0)
        , m_zoomSpeed(0.1f)
        , m_panSpeed(0.01f)
        , m_orbitSpeed(0.3f) {}
    
    /**
     * @brief 激活控制器
     */
    void Activate() {
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
        SUBSCRIBE_EVENT(MouseScrolledEvent, OnMouseScrolled);
        SUBSCRIBE_EVENT(KeyPressedEvent, OnKeyPressed);
    }
    
    /**
     * @brief 停用控制器
     */
    void Deactivate() {
        m_isPanning = false;
        m_isOrbiting = false;
        m_isZooming = false;
    }
    
    /**
     * @brief 设置相机
     */
    void SetCamera(Camera* camera) {
        m_camera = camera;
    }
    
    /**
     * @brief 获取相机
     */
    Camera* GetCamera() const {
        return m_camera;
    }
    
    /**
     * @brief 设置速度
     */
    void SetSpeeds(float zoomSpeed, float panSpeed, float orbitSpeed) {
        m_zoomSpeed = zoomSpeed;
        m_panSpeed = panSpeed;
        m_orbitSpeed = orbitSpeed;
    }
    
    /**
     * @brief 重置相机到默认位置
     */
    void ResetCamera() {
        if (m_camera) {
            m_camera->SetPosition(glm::vec3(0, 0, 5));
            m_camera->SetTarget(glm::vec3(0, 0, 0));
        }
    }
    
    /**
     * @brief 聚焦到点
     */
    void FocusOnPoint(const glm::vec3& point) {
        if (m_camera) {
            m_camera->SetTarget(point);
        }
    }

private:
    void OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        m_lastX = event.GetX();
        m_lastY = event.GetY();
        
        // 中键平移
        if (event.GetButton() == 1) {
            m_isPanning = true;
            event.SetHandled(true);
        }
        // 右键旋转
        else if (event.GetButton() == 2) {
            m_isOrbiting = true;
            event.SetHandled(true);
        }
    }
    
    void OnMouseButtonReleased(MouseButtonReleasedEvent& event) {
        if (event.GetButton() == 1) {
            m_isPanning = false;
            event.SetHandled(true);
        }
        else if (event.GetButton() == 2) {
            m_isOrbiting = false;
            event.SetHandled(true);
        }
    }
    
    void OnMouseMoved(MouseMovedEvent& event) {
        if (!m_camera) return;
        
        float deltaX = event.GetX() - m_lastX;
        float deltaY = event.GetY() - m_lastY;
        
        // 平移
        if (m_isPanning) {
            glm::vec3 right = m_camera->GetRight();
            glm::vec3 up = m_camera->GetUp();
            
            glm::vec3 panDelta = (-right * deltaX + up * deltaY) * m_panSpeed;
            m_camera->Translate(panDelta);
            
            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }
        
        // 旋转（轨道）
        if (m_isOrbiting) {
            m_camera->Rotate(-deltaX * m_orbitSpeed, -deltaY * m_orbitSpeed);
            
            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }
    }
    
    void OnMouseScrolled(MouseScrolledEvent& event) {
        if (!m_camera) return;
        
        float scrollY = event.GetYOffset();
        
        // 缩放
        m_camera->Zoom(scrollY * m_zoomSpeed);
        
        event.SetHandled(true);
    }
    
    void OnKeyPressed(KeyPressedEvent& event) {
        if (!m_camera) return;
        
        // 快捷键
        switch (event.GetKeyCode()) {
            case 'R': // 重置相机
                ResetCamera();
                break;
            case 'F': // 聚焦
                FocusOnPoint(glm::vec3(0, 0, 0));
                break;
        }
    }
    
    Camera* m_camera;
    bool m_isPanning;
    bool m_isOrbiting;
    bool m_isZooming;
    float m_lastX, m_lastY;
    float m_zoomSpeed;
    float m_panSpeed;
    float m_orbitSpeed;
};

/**
 * @brief 相机控制器管理器
 */
class CameraControllerManager {
public:
    static CameraControllerManager& Instance() {
        static CameraControllerManager instance;
        return instance;
    }
    
    /**
     * @brief 获取主控制器
     */
    CameraController& GetMainController() {
        return m_mainController;
    }
    
    /**
     * @brief 激活控制器
     */
    void Activate() {
        m_mainController.Activate();
    }
    
    /**
     * @brief 停用控制器
     */
    void Deactivate() {
        m_mainController.Deactivate();
    }

private:
    CameraControllerManager() = default;
    CameraController m_mainController{nullptr};
};
