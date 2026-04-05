#pragma once
#include "Camera.h"
#include "../event/Event.h"
#include "../event/EventBus.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 相机控制器类
 * @brief 处理相机的用户输入控制（平移/旋转/缩放）
 * @details 基于事件系统响应用户输入
 * @details 支持鼠标和键盘控制
 * @details 可自定义速度参数
 * @功能描述 实现相机的交互式控制
 * @设计目的 提供用户友好的相机操作方式
 * @使用场景 3D 编辑器中的相机导航
 * @关键实现 事件订阅模式，支持多种输入方式
 */
class CameraController
{
public:
    /**
     * @brief 构造函数
     * @param camera 要控制的相机对象
     */
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
     * @details 订阅所有相关事件
     * @details 启用事件处理
     */
    void Activate()
    {
        SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
        SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
        SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
        SUBSCRIBE_EVENT(MouseScrolledEvent, OnMouseScrolled);
        SUBSCRIBE_EVENT(KeyPressedEvent, OnKeyPressed);
    }

    /**
     * @brief 停用控制器
     * @details 重置所有状态标志
     * @details 停止响应用户输入
     */
    void Deactivate()
    {
        m_isPanning = false;
        m_isOrbiting = false;
        m_isZooming = false;
    }

    /**
     * @brief 设置相机
     * @param camera 新的相机对象
     */
    void SetCamera(Camera* camera)
    {
        m_camera = camera;
    }

    /**
     * @brief 获取相机
     * @return 当前控制的相机对象
     */
    Camera* GetCamera() const
    {
        return m_camera;
    }

    /**
     * @brief 设置速度参数
     * @param zoomSpeed 缩放速度
     * @param panSpeed 平移速度
     * @param orbitSpeed 旋转速度
     */
    void SetSpeeds(float zoomSpeed, float panSpeed, float orbitSpeed)
    {
        m_zoomSpeed = zoomSpeed;
        m_panSpeed = panSpeed;
        m_orbitSpeed = orbitSpeed;
    }

    /**
     * @brief 重置相机到默认位置
     * @details 将相机移动到初始位置和角度
     */
    void ResetCamera()
    {
        if (m_camera)
        {
            m_camera->SetPosition(glm::vec3(0, 0, 5));
            m_camera->SetTarget(glm::vec3(0, 0, 0));
        }
    }

    /**
     * @brief 聚焦到点
     * @param point 目标点坐标
     * @details 将相机的观察点设置为目标点
     */
    void FocusOnPoint(const glm::vec3& point)
    {
        if (m_camera)
        {
            m_camera->SetTarget(point);
        }
    }

private:
    /**
     * @brief 处理鼠标按下事件
     * @param event 鼠标按下事件
     * @details 中键（button=1）启动平移模式
     * @details 右键（button=2）启动旋转模式
     */
    void OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        m_lastX = event.GetX();
        m_lastY = event.GetY();

        // 中键平移
        if (event.GetButton() == 1)
        {
            m_isPanning = true;
            event.SetHandled(true);
        }
        // 右键旋转
        else if (event.GetButton() == 2)
        {
            m_isOrbiting = true;
            event.SetHandled(true);
        }
    }

    /**
     * @brief 处理鼠标释放事件
     * @param event 鼠标释放事件
     * @details 释放中键或右键，停止平移或旋转
     */
    void OnMouseButtonReleased(MouseButtonReleasedEvent& event)
    {
        if (event.GetButton() == 1)
        {
            m_isPanning = false;
            event.SetHandled(true);
        }
        else if (event.GetButton() == 2)
        {
            m_isOrbiting = false;
            event.SetHandled(true);
        }
    }

    /**
     * @brief 处理鼠标移动事件
     * @param event 鼠标移动事件
     * @details 根据当前模式执行平移或旋转操作
     */
    void OnMouseMoved(MouseMovedEvent& event)
    {
        if (!m_camera) return;

        float deltaX = event.GetX() - m_lastX;
        float deltaY = event.GetY() - m_lastY;

        // 平移
        if (m_isPanning)
        {
            glm::vec3 right = m_camera->GetRight();
            glm::vec3 up = m_camera->GetUp();

            glm::vec3 panDelta = (-right * deltaX + up * deltaY) * m_panSpeed;
            m_camera->Translate(panDelta);

            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }

        // 旋转（轨道）
        if (m_isOrbiting)
        {
            m_camera->Rotate(-deltaX * m_orbitSpeed, -deltaY * m_orbitSpeed);

            m_lastX = event.GetX();
            m_lastY = event.GetY();
            event.SetHandled(true);
        }
    }

    /**
     * @brief 处理鼠标滚轮事件
     * @param event 鼠标滚轮事件
     * @details 根据滚轮滚动值缩放相机
     */
    void OnMouseScrolled(MouseScrolledEvent& event)
    {
        if (!m_camera) return;

        float scrollY = event.GetYOffset();

        // 缩放
        m_camera->Zoom(scrollY * m_zoomSpeed);

        event.SetHandled(true);
    }

    /**
     * @brief 处理键盘按下事件
     * @param event 键盘按下事件
     * @details 支持快捷键操作：
     * @details - R: 重置相机
     * @details - F: 聚焦到原点
     */
    void OnKeyPressed(KeyPressedEvent& event)
    {
        if (!m_camera) return;

        // 快捷键
        switch (event.GetKeyCode())
        {
            case 'R':  // 重置相机
                ResetCamera();
                break;
            case 'F':  // 聚焦
                FocusOnPoint(glm::vec3(0, 0, 0));
                break;
        }
    }

    Camera* m_camera;         ///< 控制的相机对象
    bool m_isPanning;         ///< 是否正在平移
    bool m_isOrbiting;        ///< 是否正在旋转
    bool m_isZooming;         ///< 是否正在缩放
    float m_lastX, m_lastY;   ///< 上次鼠标位置
    float m_zoomSpeed;        ///< 缩放速度
    float m_panSpeed;         ///< 平移速度
    float m_orbitSpeed;       ///< 旋转速度
};

/**
 * @brief 相机控制器管理器类
 * @brief 管理全局相机控制器
 * @details 单例模式，提供全局访问点
 * @details 管理主相机控制器的生命周期
 * @功能描述 统一管理相机控制器
 * @设计目的 提供集中化的相机控制管理
 * @使用场景 应用程序中需要全局访问相机控制器时
 * @关键实现 单例模式，委托给内部控制器对象
 */
class CameraControllerManager
{
public:
    /**
     * @brief 获取单例实例
     * @return CameraControllerManager 实例引用
     */
    static CameraControllerManager& Instance()
    {
        static CameraControllerManager instance;
        return instance;
    }

    /**
     * @brief 获取主控制器
     * @return 主控制器引用
     */
    CameraController& GetMainController()
    {
        return m_mainController;
    }

    /**
     * @brief 激活控制器
     * @details 激活主控制器的事件订阅
     */
    void Activate()
    {
        m_mainController.Activate();
    }

    /**
     * @brief 停用控制器
     * @details 停用主控制器的事件订阅
     */
    void Deactivate()
    {
        m_mainController.Deactivate();
    }

private:
    CameraControllerManager() = default;  ///< 私有构造函数（单例模式）
    CameraController m_mainController{nullptr};  ///< 主控制器
};
