#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include <glm/glm.hpp>

/**
 * @brief 缩放实体命令类
 * @details 实现实体缩放操作，支持撤销/重做
 * @功能描述 封装缩放实体的操作
 * @设计目的 支持缩放操作的撤销/重做
 * @使用场景 编辑器中缩放实体
 * @关键实现 记录原始缩放和新缩放，实现 Undo/Redo
 */
class ScaleEntityCommand : public Command
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   * @param newScale 新缩放值
   */
  ScaleEntityCommand(uint32_t entityId, const glm::vec3& newScale)
    : m_entityId(entityId), m_newScale(newScale) {}

  /**
   * @brief 执行命令
   * @details 缩放实体到新缩放值
   */
  void Execute() override
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();

    for (auto& entity : entities)
    {
      if (entity.GetId() == m_entityId)
      {
        if (!m_hasOldScale)
        {
          m_oldScale = entity.GetComponent<TransformComponent>().scale;
          m_hasOldScale = true;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.scale = m_newScale;
        transform.MarkDirty();
        break;
      }
    }
  }

  /**
   * @brief 撤销命令
   * @details 恢复实体到原始缩放值
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
        transform.scale = m_oldScale;
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
    return "ScaleEntity";
  }

private:
  uint32_t m_entityId;        ///< 实体 ID
  glm::vec3 m_oldScale;       ///< 原始缩放值
  glm::vec3 m_newScale;       ///< 新缩放值
  bool m_hasOldScale = false; ///< 是否已记录原始缩放
};

/**
 * @brief 缩放工具类
 * @brief 用于缩放选中的实体
 * @details 支持轴向缩放和均匀缩放
 * @details 使用缩放 Gizmo 进行交互
 * @功能描述 实现实体缩放功能
 * @设计目的 提供用户调整实体大小的能力
 * @使用场景 编辑器中缩放实体
 * @关键实现 支持轴向选择和均匀缩放，实时更新缩放值
 */
class ScaleTool : public Tool
{
public:
  /**
   * @brief 构造函数
   */
  ScaleTool()
    : m_isDragging(false)
    , m_selectedEntityId(0)
    , m_scaleMode(2)  // 2=均匀缩放，0=X 轴，1=Y 轴，2=Z 轴
    , m_dragStartX(0)
    , m_dragStartY(0) {}

  /**
   * @brief 获取工具类型
   * @return ToolType::Scale
   */
  ToolType GetType() const override
  {
    return ToolType::Scale;
  }

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  std::string GetName() const override
  {
    return "Scale Tool";
  }

  /**
   * @brief 激活工具
   * @details 订阅鼠标和滚轮事件
   */
  void OnActivate() override
  {
    SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
    SUBSCRIBE_EVENT(MouseButtonReleasedEvent, OnMouseButtonReleased);
    SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
    SUBSCRIBE_EVENT(MouseScrolledEvent, OnMouseScrolled);
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
   * @details 渲染缩放 Gizmo（三个轴向的方块）
   */
  void Render() override
  {
    if (m_selectedEntityId != 0)
    {
      // TODO: 渲染缩放 Gizmo（三个轴向的方块）
    }
  }

  /**
   * @brief 设置缩放模式
   * @param mode 缩放模式（0=X 轴，1=Y 轴，2=Z 轴，3=均匀）
   */
  void SetScaleMode(int mode)
  {
    m_scaleMode = mode;
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
              m_originalScale = entity.GetComponent<TransformComponent>().scale;
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
        auto cmd = std::make_unique<ScaleEntityCommand>(
            m_selectedEntityId, m_currentScale);
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
      float deltaY = event.GetY() - m_dragStartY;

      // 缩放系数
      float scaleDelta = 1.0f + deltaY * 0.01f;

      m_currentScale = m_originalScale;

      switch (m_scaleMode)
      {
        case 0:  // X 轴
          m_currentScale.x *= scaleDelta;
          break;
        case 1:  // Y 轴
          m_currentScale.y *= scaleDelta;
          break;
        case 2:  // Z 轴
          m_currentScale.z *= scaleDelta;
          break;
        case 3:  // 均匀缩放
          m_currentScale *= scaleDelta;
          break;
      }

      // 确保缩放值为正
      m_currentScale.x = max(0.01f, m_currentScale.x);
      m_currentScale.y = max(0.01f, m_currentScale.y);
      m_currentScale.z = max(0.01f, m_currentScale.z);

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
            transform.scale = m_currentScale;
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
  int m_scaleMode;           ///< 缩放模式（0=X, 1=Y, 2=Z, 3=均匀）
  float m_dragStartX;        ///< 拖拽起始 X 坐标
  float m_dragStartY;        ///< 拖拽起始 Y 坐标
  glm::vec3 m_originalScale; ///< 原始缩放值
  glm::vec3 m_currentScale;  ///< 当前缩放值
};
        // 滚轮缩放（当没有拖动时）
        if (!m_isDragging && m_selectedEntityId != 0) {
            float scrollDelta = event.GetYOffset();
            float scaleDelta = 1.0f + scrollDelta * 0.1f;
            
            m_currentScale = m_originalScale * scaleDelta;
            
            // 实时更新
            auto& world = World::Instance();
            auto entities = world.GetEntities();
            
            for (auto& entity : entities) {
                if (entity.GetId() == m_selectedEntityId) {
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        transform.scale = m_currentScale;
                        transform.MarkDirty();
                    }
                    break;
                }
            }
            
            event.SetHandled(true);
        }
    }
    
    bool m_isDragging;
    uint32_t m_selectedEntityId;
    int m_scaleMode;
    float m_dragStartX, m_dragStartY;
    glm::vec3 m_originalScale;
    glm::vec3 m_currentScale;
};
