#pragma once
#include "Event.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

/**
 * @brief 事件总线类
 * @details 实现观察者模式，用于事件的发布和订阅
 * @details 支持按事件类型订阅和发布
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 管理事件的订阅和发布
 * @设计目的 解耦事件发送者和接收者
 * @使用场景 模块间通信、事件驱动编程
 * @关键实现 使用模板和类型擦除实现类型安全的事件分发
 */
class EventBus
{
public:
  /**
   * @brief 获取事件总线单例实例
   * @return 事件总线实例引用
   */
  static EventBus& Instance()
  {
    static EventBus instance;
    return instance;
  }

  /**
   * @brief 订阅事件
   * @tparam EventType 事件类型
   * @param callback 回调函数，参数为事件类型引用
   */
  template<typename EventType>
  void Subscribe(std::function<void(EventType&)> callback)
  {
    const char* eventType = EventType().GetType();

    auto wrapper = [callback](Event* event)
    {
      callback(*static_cast<EventType*>(event));
    };

    m_subscribers[eventType].push_back(wrapper);
  }

  /**
   * @brief 发布事件
   * @tparam EventType 事件类型
   * @param event 事件对象引用
   * @details 按事件类型通知所有订阅者，如果事件被标记为已处理则停止通知
   */
  template<typename EventType>
  void Publish(EventType& event)
  {
    const char* eventType = event.GetType();

    auto it = m_subscribers.find(eventType);
    if (it != m_subscribers.end())
    {
      for (auto& callback : it->second)
      {
        if (event.IsHandled()) break;
        callback(&event);
      }
    }
  }

  /**
   * @brief 发布事件（动态创建）
   * @tparam EventType 事件类型
   * @tparam Args 事件构造函数参数类型
   * @param args 事件构造函数参数
   * @details 自动创建事件对象并发布
   */
  template<typename EventType, typename... Args>
  void Publish(Args&&... args)
  {
    EventType event(std::forward<Args>(args)...);
    Publish(event);
  }

  /**
   * @brief 取消订阅
   * @tparam EventType 事件类型
   * @details 移除该类型事件的所有订阅
   */
  template<typename EventType>
  void Unsubscribe()
  {
    const char* eventType = EventType().GetType();
    m_subscribers.erase(eventType);
  }

  /**
   * @brief 清空所有订阅
   * @details 移除所有事件类型的订阅
   */
  void Clear()
  {
    m_subscribers.clear();
  }

  /**
   * @brief 获取订阅者数量
   * @param eventType 事件类型名称
   * @return 该类型事件的订阅者数量
   */
  size_t GetSubscriberCount(const std::string& eventType) const
  {
    auto it = m_subscribers.find(eventType);
    if (it != m_subscribers.end())
    {
      return it->second.size();
    }
    return 0;
  }

private:
  /**
   * @brief 私有构造函数
   */
  EventBus() = default;

  /**
   * @brief 私有析构函数
   */
  ~EventBus() = default;

  /**
   * @brief 禁用拷贝构造函数
   */
  EventBus(const EventBus&) = delete;

  /**
   * @brief 禁用赋值运算符
   */
  EventBus& operator=(const EventBus&) = delete;

  using EventCallback = std::function<void(Event*)>;  ///< 事件回调类型
  std::unordered_map<std::string, std::vector<EventCallback>> m_subscribers;  ///< 订阅者映射表
};

/**
 * @brief 便捷宏，用于在类中快速订阅事件
 * @param EventType 事件类型
 * @param handler 处理函数名
 */
#define SUBSCRIBE_EVENT(EventType, handler) \
    EventBus::Instance().Subscribe<EventType>([this](EventType& e) { handler(e); })
