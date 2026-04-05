#pragma once
#include "Tool.h"
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include "../core/command/CreateEntityCommand.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 选择工具类
 * @details 用于选择和操作场景中的实体
 * @details 支持鼠标点击选择和悬停高亮
 * @功能描述 实现实体选择功能
 * @设计目的 提供用户与场景实体交互的能力
 * @使用场景 编辑器中选择实体进行操作
 * @关键实现 使用射线投射检测实体，维护悬停实体 ID
 */
class SelectTool : public Tool
{
public:
  /**
   * @brief 构造函数
   */
  SelectTool() : m_hoveredEntityId(0) {}

  /**
   * @brief 获取工具类型
   * @return ToolType::Select
   */
  ToolType GetType() const override
  {
    return ToolType::Select;
  }

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  std::string GetName() const override
  {
    return "Select Tool";
  }

  /**
   * @brief 激活工具
   * @details 订阅鼠标事件
   */
  void OnActivate() override
  {
    // 订阅鼠标事件
    SUBSCRIBE_EVENT(MouseButtonPressedEvent, OnMouseButtonPressed);
    SUBSCRIBE_EVENT(MouseMovedEvent, OnMouseMoved);
  }

  /**
   * @brief 停用工具
   * @details 重置悬停实体 ID
   */
  void OnDeactivate() override
  {
    m_hoveredEntityId = 0;
  }

  /**
   * @brief 处理事件
   * @param event 事件引用
   * @return 事件是否已处理
   */
  bool HandleEvent(Event& event) override
  {
    // 由订阅的回调处理
    return event.IsHandled();
  }

  /**
   * @brief 渲染工具
   * @details 渲染悬停高亮效果
   */
  void Render() override
  {
    // 渲染悬停高亮
    if (m_hoveredEntityId != 0)
    {
      auto& world = World::Instance();
      auto entities = world.GetEntities();

      for (auto& entity : entities)
      {
        if (entity.GetId() == m_hoveredEntityId && entity.HasComponent<RenderComponent>())
        {
          // TODO: 渲染高亮轮廓
          break;
        }
      }
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
      float x = event.GetX();
      float y = event.GetY();

      // TODO: 实现射线投射选择逻辑
      // 这里需要根据鼠标位置进行射线投射，检测与实体的交点

      // 示例：清除之前的选择
      auto& world = World::Instance();
      auto entities = world.GetEntities();

      for (auto& entity : entities)
      {
        if (entity.HasComponent<SelectionComponent>())
        {
          auto& selection = entity.GetComponent<SelectionComponent>();
          if (selection.isSelected)
          {
            selection.isSelected = false;
            // 发布选择改变事件
            EventBus::Instance().Publish<EntitySelectionChangedEvent>(
                entity.GetId(), false);
          }
        }
      }

      // TODO: 选择新实体
      // 这里需要根据射线投射结果选择实体

      event.SetHandled(true);
    }
  }

  /**
   * @brief 鼠标移动事件处理
   * @param event 鼠标移动事件
   */
  void OnMouseMoved(MouseMovedEvent& event)
  {
    float x = event.GetX();
    float y = event.GetY();

    // TODO: 更新悬停实体
    // 这里需要根据鼠标位置更新悬停的实体

    event.SetHandled(false);  // 不阻塞其他处理
  }

  uint32_t m_hoveredEntityId;  ///< 当前悬停的实体 ID
};
