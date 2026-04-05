#pragma once
#include "Tool.h"
#include "../core/event/EventBus.h"
#include <glm/glm.hpp>

/**
 * @brief 平移工具类
 * @brief 用于平移相机视图
 * @details 通过鼠标中键拖拽实现视图平移
 * @功能描述 实现视图平移功能
 * @设计目的 提供用户调整视图位置的能力
 * @使用场景 编辑器中平移视图
 * @关键实现 使用鼠标中键拖拽，更新相机位置
 */
class PanTool : public Tool
{
public:
  /**
   * @brief 构造函数
   */
  PanTool()
    : m_isDragging(false)
    , m_lastX(0)
    , m_lastY(0) {}

  /**
   * @brief 获取工具类型
   * @return ToolType::Pan
   */
  ToolType GetType() const override
  {
    return ToolType::Pan;
  }

  /**
   * @brief 获取工具名称
   * @return 工具名称字符串
   */
  std::string GetName() const override
  {
    return "Pan Tool";
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
   * @details 重置拖拽状态
   */
  void OnDeactivate() override
  {
    m_isDragging = false;
  }

private:
  /**
   * @brief 鼠标按下事件处理
   * @param event 鼠标按下事件
   */
  void OnMouseButtonPressed(MouseButtonPressedEvent& event)
  {
    if (event.GetButton() == 1)
    {  // 中键
      m_isDragging = true;
      m_lastX = event.GetX();
      m_lastY = event.GetY();
      event.SetHandled(true);
    }
  }

  /**
   * @brief 鼠标释放事件处理
   * @param event 鼠标释放事件
   */
  void OnMouseButtonReleased(MouseButtonReleasedEvent& event)
  {
    if (event.GetButton() == 1)
    {
      m_isDragging = false;
      event.SetHandled(true);
    }
  }

  /**
   * @brief 鼠标移动事件处理
   * @param event 鼠标移动事件
   */
  void OnMouseMoved(MouseMovedEvent& event)
  {
    if (m_isDragging)
    {
      float deltaX = event.GetX() - m_lastX;
      float deltaY = event.GetY() - m_lastY;

      // TODO: 更新相机位置
      // Camera::Instance().Translate(-deltaX * 0.01f, deltaY * 0.01f, 0);

      m_lastX = event.GetX();
      m_lastY = event.GetY();
      event.SetHandled(true);
    }
  }

  bool m_isDragging;   ///< 是否正在拖拽
  float m_lastX;       ///< 上次鼠标 X 位置
  float m_lastY;       ///< 上次鼠标 Y 位置
};
