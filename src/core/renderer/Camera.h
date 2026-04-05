#pragma once
#include <glm/glm.hpp>

/**
 * @brief 相机类
 * @details 管理相机参数和视图矩阵
 * @details 提供透视投影和视图矩阵计算功能
 * @功能描述 负责相机的位置、目标和投影矩阵的计算
 * @设计目的 提供一个统一的相机管理接口，简化相机操作
 * @使用场景 用于在 3D 场景中控制视角
 * @关键实现 使用 LookAt 方法计算视图矩阵，使用透视投影计算投影矩阵
 */
class Camera
{
public:
  /**
   * @brief 构造函数
   * @details 初始化相机参数为默认值
   */
  Camera();

  /**
   * @brief 析构函数
   */
  ~Camera() = default;

  /**
   * @brief 设置相机位置
   * @param position 世界空间中的相机位置
   */
  void SetPosition(const glm::vec3& position);

  /**
   * @brief 设置相机目标点
   * @param target 相机观察的目标点位置
   */
  void SetTarget(const glm::vec3& target);

  /**
   * @brief 设置相机向上向量
   * @param up 相机的向上方向向量
   */
  void SetUp(const glm::vec3& up);

  /**
   * @brief 设置视场角
   * @param fov 视场角（单位：度）
   */
  void SetFieldOfView(float fov);

  /**
   * @brief 设置宽高比
   * @param aspect 宽高比（宽度/高度）
   */
  void SetAspectRatio(float aspect);

  /**
   * @brief 设置近裁剪面距离
   * @param nearPlane 近裁剪面距离
   */
  void SetNearPlane(float nearPlane);

  /**
   * @brief 设置远裁剪面距离
   * @param farPlane 远裁剪面距离
   */
  void SetFarPlane(float farPlane);

  /**
   * @brief 获取视图矩阵
   * @return 视图矩阵（4x4 矩阵）
   */
  glm::mat4 GetViewMatrix() const;

  /**
   * @brief 获取投影矩阵
   * @return 透视投影矩阵（4x4 矩阵）
   */
  glm::mat4 GetProjectionMatrix() const;

  /**
   * @brief 获取视图投影矩阵
   * @return 视图矩阵和投影矩阵的乘积
   */
  glm::mat4 GetViewProjectionMatrix() const;

private:
  // 相机参数
  glm::vec3 m_position;   ///< 世界空间中的相机位置
  glm::vec3 m_target;     ///< 相机观察的目标点位置
  glm::vec3 m_up;         ///< 相机的向上方向向量
  float m_fieldOfView;    ///< 视场角（单位：度）
  float m_aspectRatio;    ///< 宽高比（宽度/高度）
  float m_nearPlane;      ///< 近裁剪面距离
  float m_farPlane;       ///< 远裁剪面距离
};