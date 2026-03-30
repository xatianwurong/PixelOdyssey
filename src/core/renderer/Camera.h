#pragma once
#include <glm/glm.hpp>

/**
 * @class Camera
 * @brief 相机类
 * @details 管理相机参数和视图矩阵
 * @功能描述 负责相机的位置、目标和投影矩阵的计算
 * @设计目的 提供一个统一的相机管理接口，简化相机操作
 * @使用场景 用于在3D场景中控制视角
 * @关键实现 可以通过Set方法调整相机参数
 */
class Camera
{
public:
    Camera();
    ~Camera() = default;

    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetUp(const glm::vec3& up);
    void SetFieldOfView(float fov);
    void SetAspectRatio(float aspect);
    void SetNearPlane(float nearPlane);
    void SetFarPlane(float farPlane);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

private:
    // Camera parameters
    glm::vec3 m_position;   // Camera position in world space
    glm::vec3 m_target;     // Camera target (look-at point)
    glm::vec3 m_up;         // Camera up vector
    float m_fieldOfView;    // Field of view in degrees
    float m_aspectRatio;    // Aspect ratio (width/height)
    float m_nearPlane;      // Near clipping plane
    float m_farPlane;       // Far clipping plane
};