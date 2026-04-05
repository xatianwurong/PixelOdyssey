#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include "../core/command/CreateEntityCommand.h"
#include <glm/glm.hpp>

/**
 * @brief 移动工具类
 * @details 用于移动选中的实体
 * @details 支持拖拽操作和撤销/重做
 * @功能描述 实现实体移动功能
 * @设计目的 提供用户调整实体位置的能力
 * @使用场景 编辑器中移动实体位置
 * @关键实现 使用鼠标拖拽，实时更新位置，支持命令模式
 */
class MoveTool : public Tool
{
public:
  /**
   * @brief 构造函数
   */
  MoveTool()
    : m_isDragging(false)
    , m_selectedEntityId(0)
    , m_dragStartX(0)
    , m_dragStartY(0) {}

  /**
   * @brief 获取工具类型
   * @return ToolType::Move
   */
  ToolType GetType() const override
  {
    return ToolType::Move;
  }

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  std::string GetName() const override
  {
    return "Move Tool";
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
   * @details 渲染移动 Gizmo（三轴箭头）
   */
  void Render() override
  {
    if (m_selectedEntityId != 0)
    {
      // TODO: 渲染移动 Gizmo（三轴箭头）
    }
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

            // 保存原始位置用于命令
            if (entity.HasComponent<TransformComponent>())
            {
              m_originalPosition = entity.GetComponent<TransformComponent>().position;
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
        // 执行移动命令
        auto cmd = std::make_unique<MoveEntityCommand>(
            m_selectedEntityId, m_currentPosition);
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

      // 更新位置（需要转换为世界坐标）
      m_currentPosition = m_originalPosition + glm::vec3(deltaX * 0.01f, deltaY * 0.01f, 0);

      // 实时更新实体位置
      auto& world = World::Instance();
      auto entities = world.GetEntities();

      for (auto& entity : entities)
      {
        if (entity.GetId() == m_selectedEntityId)
        {
          if (entity.HasComponent<TransformComponent>())
          {
            auto& transform = entity.GetComponent<TransformComponent>();
            transform.position = m_currentPosition;
            transform.MarkDirty();
          }
          break;
        }
      }

      event.SetHandled(true);
    }
  }

  bool m_isDragging;           ///< 是否正在拖拽
  uint32_t m_selectedEntityId; ///< 选中的实体 ID
  float m_dragStartX;          ///< 拖拽起始 X 坐标
  float m_dragStartY;          ///< 拖拽起始 Y 坐标
  glm::vec3 m_originalPosition;   ///< 原始位置
  glm::vec3 m_currentPosition;    ///< 当前位置
};
