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
 */
class EventBus {
public:
    /**
     * @brief 获取单例实例
     */
    static EventBus& Instance() {
        static EventBus instance;
        return instance;
    }
    
    /**
     * @brief 订阅事件
     * @tparam EventType 事件类型
     * @param callback 回调函数
     */
    template<typename EventType>
    void Subscribe(std::function<void(EventType&)> callback) {
        const char* eventType = EventType().GetType();
        
        auto wrapper = [callback](Event* event) {
            callback(*static_cast<EventType*>(event));
        };
        
        m_subscribers[eventType].push_back(wrapper);
    }
    
    /**
     * @brief 发布事件
     * @tparam EventType 事件类型
     * @param event 事件对象
     */
    template<typename EventType>
    void Publish(EventType& event) {
        const char* eventType = event.GetType();
        
        auto it = m_subscribers.find(eventType);
        if (it != m_subscribers.end()) {
            for (auto& callback : it->second) {
                if (event.IsHandled()) break;
                callback(&event);
            }
        }
    }
    
    /**
     * @brief 发布事件（动态创建）
     * @tparam EventType 事件类型
     * @param args 事件构造函数参数
     */
    template<typename EventType, typename... Args>
    void Publish(Args&&... args) {
        EventType event(std::forward<Args>(args)...);
        Publish(event);
    }
    
    /**
     * @brief 取消订阅
     * @tparam EventType 事件类型
     */
    template<typename EventType>
    void Unsubscribe() {
        const char* eventType = EventType().GetType();
        m_subscribers.erase(eventType);
    }
    
    /**
     * @brief 清空所有订阅
     */
    void Clear() {
        m_subscribers.clear();
    }
    
    /**
     * @brief 获取订阅者数量
     * @param eventType 事件类型
     * @return 订阅者数量
     */
    size_t GetSubscriberCount(const std::string& eventType) const {
        auto it = m_subscribers.find(eventType);
        if (it != m_subscribers.end()) {
            return it->second.size();
        }
        return 0;
    }

private:
    EventBus() = default;
    ~Bus() = default;
    
    // 禁止拷贝
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
    using EventCallback = std::function<void(Event*)>;
    std::unordered_map<std::string, std::vector<EventCallback>> m_subscribers;
};

// 便捷宏，用于在类中快速订阅事件
#define SUBSCRIBE_EVENT(EventType, handler) \
    EventBus::Instance().Subscribe<EventType>([this](EventType& e) { handler(e); })
