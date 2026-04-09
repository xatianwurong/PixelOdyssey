#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

/**
 * @brief 组件基类
 * @details ECS 架构中的组件基类，所有组件必须继承此类
 * @details 提供类型识别接口
 * @功能描述 作为所有组件的抽象基类
 * @设计目的 实现组件的多态性和类型识别
 * @使用场景 所有 ECS 组件都必须继承此类
 */
class Component
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~Component() = default;

  /**
   * @brief 获取组件类型名称
   * @return 组件类型字符串
   */
  virtual const char* GetType() const = 0;
};

/**
 * @brief 变换组件
 * @details 存储实体的位置、旋转和缩放信息
 * @details 支持延迟计算变换矩阵（脏标记模式）
 * @功能描述 管理实体的空间变换数据
 * @设计目的 提供高效的变换矩阵计算机制
 * @使用场景 任何需要空间变换的实体都需要此组件
 * @关键实现 使用 isDirty 标记避免不必要的矩阵计算
 */
class TransformComponent : public Component
{
public:
  glm::vec3 position{0.0f, 0.0f, 0.0f};  ///< 位置（世界坐标）
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};  ///< 旋转（欧拉角，单位：度）
  glm::vec3 scale{1.0f, 1.0f, 1.0f};     ///< 缩放（各轴向缩放比例）

  bool isDirty = true;                   ///< 脏标记，标记是否需要更新变换矩阵
  glm::mat4 modelMatrix{1.0f};           ///< 变换矩阵（缓存）

  /**
   * @brief 获取组件类型
   * @return 组件类型字符串
   */
  const char* GetType() const override { return "TransformComponent"; }

  /**
   * @brief 获取变换矩阵
   * @details 根据位置、旋转、缩放计算模型矩阵
   * @details 使用延迟计算优化性能
   * @return 4x4 变换矩阵
   */
  glm::mat4 GetModelMatrix();

  /**
   * @brief 标记变换为脏
   * @details 当变换数据改变时调用，下次获取矩阵时会重新计算
   */
  void MarkDirty() { isDirty = true; }
};

/**
 * @brief 渲染组件
 * @details 存储实体的渲染相关数据
 * @功能描述 管理实体的渲染属性和状态
 * @设计目的 控制实体的可见性和渲染顺序
 * @使用场景 任何需要渲染的实体都需要此组件
 */
class RenderComponent : public Component
{
public:
  std::string meshId;         ///< 网格资源 ID
  std::string materialId;     ///< 材质资源 ID
  bool isVisible = true;      ///< 是否可见
  int renderOrder = 0;        ///< 渲染顺序（值越小越先渲染）

  /**
   * @brief 获取组件类型
   * @return 组件类型字符串
   */
  const char* GetType() const override { return "RenderComponent"; }
};

/**
 * @brief 名称组件
 * @details 为实体提供可读的名称
 * @功能描述 存储实体的用户友好名称
 * @设计目的 便于调试和 UI 显示
 * @使用场景 需要显示或引用实体名称的场景
 */
class NameComponent : public Component
{
public:
  std::string name;  ///< 实体名称

  /**
   * @brief 获取组件类型
   * @return 组件类型字符串
   */
  const char* GetType() const override { return "NameComponent"; }
};

/**
 * @brief 选择组件
 * @details 标记实体是否被选中
 * @功能描述 管理实体的选择状态和高亮颜色
 * @设计目的 支持编辑器中的实体选择功能
 * @使用场景 编辑器中选择、高亮显示实体
 */
class SelectionComponent : public Component
{
public:
  bool isSelected = false;                      ///< 是否被选中
  glm::vec4 selectionColor{1.0f, 0.5f, 0.0f, 1.0f};  ///< 选中时的高亮颜色（橙色）

  /**
   * @brief 获取组件类型
   * @return 组件类型字符串
   */
  const char* GetType() const override { return "SelectionComponent"; }
};

/**
 * @brief 获取变换矩阵（内联实现）
 * @details 根据位置、旋转、缩放计算模型矩阵
 * @details 变换顺序：缩放 -> 旋转 X -> 旋转 Y -> 旋转 Z -> 平移
 * @return 4x4 变换矩阵
 */
inline glm::mat4 TransformComponent::GetModelMatrix()
{
  if (isDirty)
  {
    glm::mat4 pos = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);

    modelMatrix = pos * rotZ * rotY * rotX * scl;
    isDirty = false;
  }
  return modelMatrix;
}
