#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 旋转实体命令类
 * @details 实现实体旋转操作，支持撤销/重做
 * @功能描述 封装旋转实体的操作
 * @设计目的 支持旋转操作的撤销/重做
 * @使用场景 编辑器中旋转实体
 * @关键实现 记录原始角度和新角度，实现 Undo/Redo
 */
class RotateEntityCommand : public Command
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   * @param newRotation 新旋转角度（欧拉角，单位：度）
   */
  RotateEntityCommand(uint32_t entityId, const glm::vec3& newRotation)
    : m_entityId(entityId), m_newRotation(newRotation) {}

  /**
   * @brief 执行命令
   * @details 旋转实体到新角度
   */
  void Execute() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        if (!m_hasOldRotation)
        {
          m_oldRotation = entity.GetComponent<TransformComponent>().rotation;
          m_hasOldRotation = true;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.rotation = m_newRotation;
        transform.MarkDirty();
        break;
      }
    }
  }

  /**
   * @brief 撤销命令
   * @details 恢复实体到原始角度
   */
  void Undo() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.rotation = m_oldRotation;
        transform.MarkDirty();
        break;
      }
    }
  }

  /**
   * @brief 获取命令名称
   * @return 命令名称字符串
   */
  std::string GetName() const override
  {
    return "RotateEntity";
  }

private:
  uint32_t m_entityId;           ///< 实体 ID
  glm::vec3 m_oldRotation;       ///< 原始旋转角度
  glm::vec3 m_newRotation;       ///< 新旋转角度
  bool m_hasOldRotation = false; ///< 是否已记录原始角度
};

/**
 * @brief 旋转工具类
 * @details 用于旋转选中的实体
 * @details 支持 X/Y/Z 三轴旋转
 * @details 使用旋转 Gizmo 进行交互
 * @功能描述 实现实体旋转功能
 * @设计目的 提供用户调整实体旋转的能力
 * @使用场景 编辑器中旋转实体
 * @关键实现 支持轴向选择，实时更新旋转角度
 */
class RotateTool : public Tool
{
public:
  /**
   * @brief 构造函数
   */
  RotateTool()
    : m_isDragging(false)
    , m_selectedEntityId(0)
    , m_rotationMode(0)  // 0=X, 1=Y, 2=Z
    , m_dragStartX(0)
    , m_dragStartY(0) {}

  /**
   * @brief 获取工具类型
   * @return ToolType::Rotate
   */
  ToolType GetType() const override
  {
    return ToolType::Rotate;
  }

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  std::string GetName() const override
  {
    return "Rotate Tool";
  }

  /**
   * @brief 激活工具
   * @details 订阅鼠标事件
   */
  void OnActivate() override
  {
    SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
    SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
    SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
  }

  /**
   * @brief 停用工具
   * @details 重置状态
   */
  void OnDeactivate() override
  {
    m_isDragging = false;
    m_selectedEntityId = 0;
  }

  /**
   * @brief 渲染工具
   * @details 渲染旋转 Gizmo（三个圆环）
   */
  void Render() override
  {
    if (m_selectedEntityId != 0)
    {
      // TODO: 渲染旋转 Gizmo（三个圆环）
    }
  }

  /**
   * @brief 设置旋转模式
   * @param mode 旋转轴模式（0=X 轴，1=Y 轴，2=Z 轴）
   */
  void SetRotationMode(int mode)
  {
    m_rotationMode = mode;
  }

private:
  /**
   * @brief 鼠标按下事件处理
   * @param event 鼠标按下事件
   */
  void OnMouseButtonPressed(MouseButtonPressedEvent& event)
  {
    if (event.GetButton() == 0)
    {  // 左键
      m_isDragging = true;
      m_dragStartX = event.GetX();
      m_dragStartY = event.GetY();

      // 获取选中的实体
      auto& world = World::Instance();
      auto entities = world.GetEntities();

      for (auto& entity : entities)
      {
        if (entity.HasComponent<SelectionComponent>())
        {
          auto& selection = entity.GetComponent<SelectionComponent>();
          if (selection.isSelected)
          {
            m_selectedEntityId = entity.GetId();

            if (entity.HasComponent<TransformComponent>())
            {
              m_originalRotation = entity.GetComponent<TransformComponent>().rotation;
            }
            break;
          }
        }
      }

      event.SetHandled(true);
    }
  }

  /**
   * @brief 鼠标释放事件处理
   * @param event 鼠标释放事件
   */
  void OnMouseButtonReleased(MouseButtonReleasedEvent& event)
  {
    if (event.GetButton() == 0 && m_isDragging)
    {
      m_isDragging = false;

      if (m_selectedEntityId != 0)
      {
        auto cmd = std::make_unique<RotateEntityCommand>(
            m_selectedEntityId, m_currentRotation);
        CommandStack::Instance().Execute(std::move(cmd));
      }

      m_selectedEntityId = 0;
      event.SetHandled(true);
    }
  }

  /**
   * @brief 鼠标移动事件处理
   * @param event 鼠标移动事件
   */
  void OnMouseMoved(MouseMovedEvent& event)
  {
    if (m_isDragging && m_selectedEntityId != 0)
    {
      float deltaX = event.GetX() - m_dragStartX;
      float deltaY = event.GetY() - m_dragStartY;

      // 根据旋转模式更新角度
      float angleDelta = 0.5f;  // 每像素 0.5 度

      m_currentRotation = m_originalRotation;

      switch (m_rotationMode)
      {
        case 0:  // X 轴
          m_currentRotation.x += deltaY * angleDelta;
          break;
        case 1:  // Y 轴
          m_currentRotation.y += deltaX * angleDelta;
          break;
        case 2:  // Z 轴
          m_currentRotation.z += deltaX * angleDelta;
          break;
      }

      // 实时更新
      auto& world = World::Instance();
      auto entities = world.GetEntities();

      for (auto& entity : entities)
      {
        if (entity.GetId() == m_selectedEntityId)
        {
          if (entity.HasComponent<TransformComponent>())
          {
            auto& transform = entity.GetComponent<TransformComponent>();
            transform.rotation = m_currentRotation;
            transform.MarkDirty();
          }
          break;
        }
      }

      event.SetHandled(true);
    }
  }

  bool m_isDragging;         ///< 是否正在拖拽
  uint32_t m_selectedEntityId;  ///< 选中的实体 ID
  int m_rotationMode;        ///< 旋转模式（0=X, 1=Y, 2=Z）
  float m_dragStartX;        ///< 拖拽起始 X 坐标
  float m_dragStartY;        ///< 拖拽起始 Y 坐标
  glm::vec3 m_originalRotation; ///< 原始旋转角度
  glm::vec3 m_currentRotation;  ///< 当前旋转角度
};
