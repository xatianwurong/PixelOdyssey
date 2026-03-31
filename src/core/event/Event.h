#pragma once
#include <string>
#include <glm/glm.hpp>

/**
 * @brief 事件基类
 * @details 所有事件必须继承此类
 */
class Event {
public:
    virtual ~Event() = default;
    
    /**
     * @brief 获取事件类型名称
     * @return 事件类型
     */
    virtual const char* GetType() const = 0;
    
    /**
     * @brief 获取事件类别
     * @return 事件类别
     */
    virtual int GetCategory() const = 0;
    
    /**
     * @brief 检查事件是否已处理
     * @return 如果已处理返回 true
     */
    bool IsHandled() const { return m_handled; }
    
    /**
     * @brief 标记事件为已处理
     */
    void SetHandled(bool handled = true) { m_handled = handled; }

protected:
    bool m_handled = false;
};

/**
 * @brief 事件类别
 */
enum class EventCategory {
    None = 0,
    Input = 1 << 0,     // 输入事件
    Mouse = 1 << 1,     // 鼠标事件
    Keyboard = 1 << 2,  // 键盘事件
    Window = 1 << 3,    // 窗口事件
    Application = 1 << 4, // 应用事件
    Entity = 1 << 5,    // 实体事件
    Scene = 1 << 6      // 场景事件
};

// 位运算操作符
inline EventCategory operator|(EventCategory a, EventCategory b) {
    return static_cast<EventCategory>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool operator&(EventCategory a, EventCategory b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

// ==================== 鼠标事件 ====================

/**
 * @brief 鼠标移动事件
 */
class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : m_x(x), m_y(y) {}
    
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    
    const char* GetType() const override { return "MouseMovedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }
    
private:
    float m_x, m_y;
};

/**
 * @brief 鼠标按下事件
 */
class MouseButtonPressedEvent : public Event {
public:
    MouseButtonPressedEvent(int button, float x, float y) 
        : m_button(button), m_x(x), m_y(y) {}
    
    int GetButton() const { return m_button; }
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    
    const char* GetType() const override { return "MouseButtonPressedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }
    
private:
    int m_button;
    float m_x, m_y;
};

/**
 * @brief 鼠标释放事件
 */
class MouseButtonReleasedEvent : public Event {
public:
    MouseButtonReleasedEvent(int button, float x, float y) 
        : m_button(button), m_x(x), m_y(y) {}
    
    int GetButton() const { return m_button; }
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    
    const char* GetType() const override { return "MouseButtonReleasedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }
    
private:
    int m_button;
    float m_x, m_y;
};

/**
 * @brief 鼠标滚轮事件
 */
class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) 
        : m_xOffset(xOffset), m_yOffset(yOffset) {}
    
    float GetXOffset() const { return m_xOffset; }
    float GetYOffset() const { return m_yOffset; }
    
    const char* GetType() const override { return "MouseScrolledEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Mouse); }
    
private:
    float m_xOffset, m_yOffset;
};

// ==================== 键盘事件 ====================

/**
 * @brief 键盘按下事件
 */
class KeyPressedEvent : public Event {
public:
    KeyPressedEvent(int keyCode, bool isRepeat = false) 
        : m_keyCode(keyCode), m_isRepeat(isRepeat) {}
    
    int GetKeyCode() const { return m_keyCode; }
    bool IsRepeat() const { return m_isRepeat; }
    
    const char* GetType() const override { return "KeyPressedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Keyboard); }
    
private:
    int m_keyCode;
    bool m_isRepeat;
};

/**
 * @brief 键盘释放事件
 */
class KeyReleasedEvent : public Event {
public:
    KeyReleasedEvent(int keyCode) : m_keyCode(keyCode) {}
    
    int GetKeyCode() const { return m_keyCode; }
    
    const char* GetType() const override { return "KeyReleasedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Input | EventCategory::Keyboard); }
    
private:
    int m_keyCode;
};

// ==================== 实体事件 ====================

/**
 * @brief 实体创建事件
 */
class EntityCreatedEvent : public Event {
public:
    EntityCreatedEvent(uint32_t entityId) : m_entityId(entityId) {}
    
    uint32_t GetEntityId() const { return m_entityId; }
    
    const char* GetType() const override { return "EntityCreatedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }
    
private:
    uint32_t m_entityId;
};

/**
 * @brief 实体销毁事件
 */
class EntityDestroyedEvent : public Event {
public:
    EntityDestroyedEvent(uint32_t entityId) : m_entityId(entityId) {}
    
    uint32_t GetEntityId() const { return m_entityId; }
    
    const char* GetType() const override { return "EntityDestroyedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }
    
private:
    uint32_t m_entityId;
};

/**
 * @brief 实体变换改变事件
 */
class EntityTransformChangedEvent : public Event {
public:
    EntityTransformChangedEvent(uint32_t entityId) : m_entityId(entityId) {}
    
    uint32_t GetEntityId() const { return m_entityId; }
    
    const char* GetType() const override { return "EntityTransformChangedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }
    
private:
    uint32_t m_entityId;
};

/**
 * @brief 实体选择改变事件
 */
class EntitySelectionChangedEvent : public Event {
public:
    EntitySelectionChangedEvent(uint32_t entityId, bool isSelected) 
        : m_entityId(entityId), m_isSelected(isSelected) {}
    
    uint32_t GetEntityId() const { return m_entityId; }
    bool IsSelected() const { return m_isSelected; }
    
    const char* GetType() const override { return "EntitySelectionChangedEvent"; }
    int GetCategory() const override { return static_cast<int>(EventCategory::Entity); }
    
private:
    uint32_t m_entityId;
    bool m_isSelected;
};
