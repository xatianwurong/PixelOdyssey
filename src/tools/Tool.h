#pragma once
#include "../core/event/Event.h"
#include <memory>
#include <string>

/**
 * @brief 工具类型枚举
 */
enum class ToolType {
    None = 0,
    Select,
    Move,
    Rotate,
    Scale,
    CreateShape,
    CreateText,
    Pan,
    Orbit
};

/**
 * @brief 工具基类
 * @details 所有工具必须继承此类
 */
class Tool {
public:
    virtual ~Tool() = default;
    
    /**
     * @brief 获取工具类型
     * @return 工具类型
     */
    virtual ToolType GetType() const = 0;
    
    /**
     * @brief 获取工具名称
     * @return 工具名称
     */
    virtual std::string GetName() const = 0;
    
    /**
     * @brief 工具激活时调用
     */
    virtual void OnActivate() {}
    
    /**
     * @brief 工具停用时调用
     */
    virtual void OnDeactivate() {}
    
    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 如果事件已处理返回 true
     */
    virtual bool HandleEvent(Event& event) {
        return false;
    }
    
    /**
     * @brief 每帧更新
     * @param deltaTime 帧间隔时间
     */
    virtual void Update(float deltaTime) {}
    
    /**
     * @brief 渲染工具 UI
     */
    virtual void Render() {}
};

/**
 * @brief 工具管理器
 * @details 管理所有工具的切换和使用
 */
class ToolManager {
public:
    static ToolManager& Instance() {
        static ToolManager instance;
        return instance;
    }
    
    /**
     * @brief 设置当前工具
     * @param tool 工具对象
     */
    void SetTool(std::unique_ptr<Tool> tool) {
        if (m_currentTool) {
            m_currentTool->OnDeactivate();
        }
        
        m_currentTool = std::move(tool);
        
        if (m_currentTool) {
            m_currentTool->OnActivate();
        }
    }
    
    /**
     * @brief 获取当前工具
     * @return 当前工具指针
     */
    Tool* GetCurrentTool() const {
        return m_currentTool.get();
    }
    
    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 如果事件已处理返回 true
     */
    bool HandleEvent(Event& event) {
        if (m_currentTool) {
            return m_currentTool->HandleEvent(event);
        }
        return false;
    }
    
    /**
     * @brief 更新工具
     * @param deltaTime 帧间隔时间
     */
    void Update(float deltaTime) {
        if (m_currentTool) {
            m_currentTool->Update(deltaTime);
        }
    }
    
    /**
     * @brief 渲染工具
     */
    void Render() {
        if (m_currentTool) {
            m_currentTool->Render();
        }
    }

private:
    ToolManager() = default;
    std::unique_ptr<Tool> m_currentTool;
};
