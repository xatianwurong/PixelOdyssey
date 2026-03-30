#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : m_position(glm::vec3(4, 3, -3)),
      m_target(glm::vec3(0, 0, 0)),
      m_up(glm::vec3(0, 1, 0)),
      m_fieldOfView(45.0f),
      m_aspectRatio(4.0f / 3.0f),
      m_nearPlane(0.1f),
      m_farPlane(100.0f)
{
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void Camera::SetTarget(const glm::vec3& target)
{
    m_target = target;
}

void Camera::SetUp(const glm::vec3& up)
{
    m_up = up;
}

void Camera::SetFieldOfView(float fov)
{
    m_fieldOfView = fov;
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspectRatio = aspect;
}

void Camera::SetNearPlane(float nearPlane)
{
    m_nearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
    m_farPlane = farPlane;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_fieldOfView), m_aspectRatio, m_nearPlane, m_farPlane);
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
    return GetProjectionMatrix() * GetViewMatrix();
}
