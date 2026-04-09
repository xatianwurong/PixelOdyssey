#pragma once
#include <string>
#include <glm/glm.hpp>

/**
 * @brief 事件基类
 * @details 所有事件必须继承此类
 * @details 提供事件类型识别和处理状态管理
 * @功能描述 作为事件系统的抽象基类
 * @设计目的 实现观察者模式，支持事件的发布和订阅
 * @使用场景 用户输入、实体变化、场景切换等各种事件的传递
 * @关键实现 使用虚函数接口，支持多态
 */
class Event
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~Event() = default;

  /**
   * @brief 获取事件类型名称
   * @return 事件类型字符串
   */
  virtual const char* GetType() const = 0;

  /**
   * @brief 获取事件类别
   * @return 事件类别标识符
   */
  virtual int GetCategory() const = 0;

  /**
   * @brief 检查事件是否已处理
   * @return 如果事件已被处理返回 true
   */
  bool IsHandled() const { return m_handled; }

  /**
   * @brief 标记事件为已处理
   * @param handled 是否已处理（默认 true）
   */
  void SetHandled(bool handled = true) { m_handled = handled; }

protected:
  bool m_handled = false;  ///< 事件处理状态标记
};

/**
 * @brief 事件类别枚举
 * @details 使用位掩码方式定义事件类别
 * @details 支持类别的组合（如 Input | Mouse）
 */
enum class EventCategory
{
  None = 0,
  Input = 1 << 0,       ///< 输入事件
  Mouse = 1 << 1,       ///< 鼠标事件
  Keyboard = 1 << 2,    ///< 键盘事件
  Window = 1 << 3,      ///< 窗口事件
  Application = 1 << 4, ///< 应用事件
  Entity = 1 << 5,      ///< 实体事件
  Scene = 1 << 6        ///< 场景事件
};

/**
 * @brief 事件类别按位或运算
 */
inline EventCategory operator|(EventCategory a, EventCategory b)
{
  return static_cast<EventCategory>(static_cast<int>(a) | static_cast<int>(b));
}

/**
 * @brief 事件类别按位与运算
 */
inline bool operator&(EventCategory a, EventCategory b)
{
  return static_cast<int>(a) & static_cast<int>(b);
}

// ==================== 鼠标事件 ====================

/**
 * @brief 鼠标移动事件
 * @details 当鼠标位置发生变化时触发
 */
class MouseMovedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param x 鼠标 X 坐标
   * @param y 鼠标 Y 坐标
   */
  MouseMovedEvent(float x, float y) : m_x(x), m_y(y) {}

  /**
   * @brief 获取 X 坐标
   * @return 鼠标 X 坐标
   */
  float GetX() const { return m_x; }

  /**
   * @brief 获取 Y 坐标
   * @return 鼠标 Y 坐标
   */
  float GetY() const { return m_y; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "MouseMovedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Mouse）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }

private:
  float m_x, m_y;  ///< 鼠标位置坐标
};

/**
 * @brief 鼠标按下事件
 * @details 当鼠标按钮被按下时触发
 */
class MouseButtonPressedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param button 鼠标按钮（0=左键，1=中键，2=右键）
   * @param x 鼠标 X 坐标
   * @param y 鼠标 Y 坐标
   */
  MouseButtonPressedEvent(int button, float x, float y)
    : m_button(button), m_x(x), m_y(y) {}

  /**
   * @brief 获取鼠标按钮
   * @return 鼠标按钮编号
   */
  int GetButton() const { return m_button; }

  /**
   * @brief 获取 X 坐标
   * @return 鼠标 X 坐标
   */
  float GetX() const { return m_x; }

  /**
   * @brief 获取 Y 坐标
   * @return 鼠标 Y 坐标
   */
  float GetY() const { return m_y; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "MouseButtonPressedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Mouse）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }

private:
  int m_button;      ///< 鼠标按钮编号
  float m_x, m_y;    ///< 鼠标位置坐标
};

/**
 * @brief 鼠标释放事件
 * @details 当鼠标按钮被释放时触发
 */
class MouseButtonReleasedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param button 鼠标按钮（0=左键，1=中键，2=右键）
   * @param x 鼠标 X 坐标
   * @param y 鼠标 Y 坐标
   */
  MouseButtonReleasedEvent(int button, float x, float y)
    : m_button(button), m_x(x), m_y(y) {}

  /**
   * @brief 获取鼠标按钮
   * @return 鼠标按钮编号
   */
  int GetButton() const { return m_button; }

  /**
   * @brief 获取 X 坐标
   * @return 鼠标 X 坐标
   */
  float GetX() const { return m_x; }

  /**
   * @brief 获取 Y 坐标
   * @return 鼠标 Y 坐标
   */
  float GetY() const { return m_y; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "MouseButtonReleasedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Mouse）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }

private:
  int m_button;      ///< 鼠标按钮编号
  float m_x, m_y;    ///< 鼠标位置坐标
};

/**
 * @brief 鼠标滚轮事件
 * @details 当鼠标滚轮滚动时触发
 */
class MouseScrolledEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param xOffset X 轴滚动量
   * @param yOffset Y 轴滚动量
   */
  MouseScrolledEvent(float xOffset, float yOffset)
    : m_xOffset(xOffset), m_yOffset(yOffset) {}

  /**
   * @brief 获取 X 轴滚动量
   * @return X 轴滚动偏移量
   */
  float GetXOffset() const { return m_xOffset; }

  /**
   * @brief 获取 Y 轴滚动量
   * @return Y 轴滚动偏移量
   */
  float GetYOffset() const { return m_yOffset; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "MouseScrolledEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Mouse）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }

private:
  float m_xOffset, m_yOffset;  ///< 滚轮滚动偏移量
};

// ==================== 键盘事件 ====================

/**
 * @brief 键盘按下事件
 * @details 当键盘按键被按下时触发
 */
class KeyPressedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param keyCode 按键代码
   * @param isRepeat 是否为重复按键（长按时）
   */
  KeyPressedEvent(int keyCode, bool isRepeat = false)
    : m_keyCode(keyCode), m_isRepeat(isRepeat) {}

  /**
   * @brief 获取按键代码
   * @return 按键代码
   */
  int GetKeyCode() const { return m_keyCode; }

  /**
   * @brief 检查是否为重复按键
   * @return 如果是重复按键返回 true
   */
  bool IsRepeat() const { return m_isRepeat; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "KeyPressedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Keyboard）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Keyboard); }

private:
  int m_keyCode;     ///< 按键代码
  bool m_isRepeat;   ///< 是否为重复按键
};

/**
 * @brief 键盘释放事件
 * @details 当键盘按键被释放时触发
 */
class KeyReleasedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param keyCode 按键代码
   */
  KeyReleasedEvent(int keyCode) : m_keyCode(keyCode) {}

  /**
   * @brief 获取按键代码
   * @return 按键代码
   */
  int GetKeyCode() const { return m_keyCode; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "KeyReleasedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Input | Keyboard）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Keyboard); }

private:
  int m_keyCode;  ///< 按键代码
};

// ==================== 实体事件 ====================

/**
 * @brief 实体创建事件
 * @details 当新实体被创建时触发
 */
class EntityCreatedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   */
  EntityCreatedEvent(uint32_t entityId) : m_entityId(entityId) {}

  /**
   * @brief 获取实体 ID
   * @return 实体 ID
   */
  uint32_t GetEntityId() const { return m_entityId; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "EntityCreatedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Entity）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }

private:
  uint32_t m_entityId;  ///< 实体 ID
};

/**
 * @brief 实体销毁事件
 * @details 当实体被销毁时触发
 */
class EntityDestroyedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   */
  EntityDestroyedEvent(uint32_t entityId) : m_entityId(entityId) {}

  /**
   * @brief 获取实体 ID
   * @return 实体 ID
   */
  uint32_t GetEntityId() const { return m_entityId; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "EntityDestroyedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Entity）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }

private:
  uint32_t m_entityId;  ///< 实体 ID
};

/**
 * @brief 实体变换改变事件
 * @details 当实体的位置、旋转或缩放发生变化时触发
 */
class EntityTransformChangedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   */
  EntityTransformChangedEvent(uint32_t entityId) : m_entityId(entityId) {}

  /**
   * @brief 获取实体 ID
   * @return 实体 ID
   */
  uint32_t GetEntityId() const { return m_entityId; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "EntityTransformChangedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Entity）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }

private:
  uint32_t m_entityId;  ///< 实体 ID
};

/**
 * @brief 实体选择改变事件
 * @details 当实体的选择状态发生变化时触发
 */
class EntitySelectionChangedEvent : public Event
{
public:
  /**
   * @brief 构造函数
   * @param entityId 实体 ID
   * @param isSelected 是否被选中
   */
  EntitySelectionChangedEvent(uint32_t entityId, bool isSelected)
    : m_entityId(entityId), m_isSelected(isSelected) {}

  /**
   * @brief 获取实体 ID
   * @return 实体 ID
   */
  uint32_t GetEntityId() const { return m_entityId; }

  /**
   * @brief 检查是否被选中
   * @return 如果实体被选中返回 true
   */
  bool IsSelected() const { return m_isSelected; }

  /**
   * @brief 获取事件类型
   * @return 事件类型字符串
   */
  const char* GetType() const override { return "EntitySelectionChangedEvent"; }

  /**
   * @brief 获取事件类别
   * @return 事件类别（Entity）
   */
  int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }

private:
  uint32_t m_entityId;  ///< 实体 ID
  bool m_isSelected;    ///< 选择状态
};
