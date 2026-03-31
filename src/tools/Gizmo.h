#pragma once
#include "../core/ecs/Entity.h"
#include "../core/ecs/Component.h"
#include "../core/renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

/**
 * @brief Gizmo 类型
 */
enum class GizmoType {
    None = 0,
    Move,
    Rotate,
    Scale
};

/**
 * @brief Gizmo 轴向
 */
enum class GizmoAxis {
    None = 0,
    X,
    Y,
    Z,
    XY,
    XZ,
    YZ,
    XYZ
};

/**
 * @brief Gizmo 类
 * @details 渲染和操作变换 Gizmo（移动/旋转/缩放手柄）
 */
class Gizmo {
public:
    Gizmo() 
        : m_gizmoType(GizmoType::None)
        , m_activeAxis(GizmoAxis::None)
        , m_hoveredAxis(GizmoAxis::None)
        , m_isDragging(false)
        , m_snapEnabled(false)
        , m_snapValue(1.0f) {}
    
    /**
     * @brief 设置 Gizmo 类型
     * @param type Gizmo 类型
     */
    void SetType(GizmoType type) {
        m_gizmoType = type;
    }
    
    /**
     * @brief 获取 Gizmo 类型
     */
    GizmoType GetType() const {
        return m_gizmoType;
    }
    
    /**
     * @brief 设置目标实体
     * @param entity 目标实体
     */
    void SetTarget(Entity entity) {
        m_targetEntity = entity;
    }
    
    /**
     * @brief 获取目标实体
     */
    Entity GetTarget() const {
        return m_targetEntity;
    }
    
    /**
     * @brief 启用/禁用网格吸附
     * @param enabled 是否启用
     */
    void SetSnapEnabled(bool enabled) {
        m_snapEnabled = enabled;
    }
    
    /**
     * @brief 设置吸附值
     * @param value 吸附值
     */
    void SetSnapValue(float value) {
        m_snapValue = value;
    }
    
    /**
     * @brief 更新 Gizmo 状态
     * @param camera 相机对象
     * @param mouseX 鼠标 X
     * @param mouseY 鼠标 Y
     */
    void Update(Camera* camera, float mouseX, float mouseY) {
        if (!m_targetEntity.IsValid()) return;
        
        // 更新 Gizmo 位置跟随目标实体
        if (m_targetEntity.HasComponent<TransformComponent>()) {
            m_position = m_targetEntity.GetComponent<TransformComponent>().position;
        }
    }
    
    /**
     * @brief 渲染 Gizmo
     */
    void Render() {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None) {
            return;
        }
        
        // TODO: 实现 Gizmo 渲染
        // 这里需要绘制：
        // - Move: 三个轴向的箭头
        // - Rotate: 三个轴向的圆环
        // - Scale: 三个轴向的方块
        
        // 示例：绘制 Gizmo 位置
        // glDisable(GL_DEPTH_TEST);
        // ... 绘制代码 ...
        // glEnable(GL_DEPTH_TEST);
    }
    
    /**
     * @brief 处理鼠标移动
     * @param camera 相机对象
     * @param mouseX 鼠标 X
     * @param mouseY 鼠标 Y
     * @return 如果处理了事件返回 true
     */
    bool HandleMouseMove(Camera* camera, float mouseX, float mouseY) {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None) {
            return false;
        }
        
        // TODO: 实现鼠标悬停检测
        // 检测鼠标是否悬停在 Gizmo 的某个轴上
        
        return false;
    }
    
    /**
     * @brief 处理鼠标按下
     * @param camera 相机对象
     * @param mouseX 鼠标 X
     * @param mouseY 鼠标 Y
     * @param button 鼠标按钮
     * @return 如果处理了事件返回 true
     */
    bool HandleMousePress(Camera* camera, float mouseX, float mouseY, int button) {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None) {
            return false;
        }
        
        if (button == 0) { // 左键
            // TODO: 检测点击了哪个轴
            // 如果点击了轴，开始拖动
            
            m_isDragging = true;
            m_dragStartX = mouseX;
            m_dragStartY = mouseY;
            
            if (m_targetEntity.HasComponent<TransformComponent>()) {
                m_dragStartValue = m_targetEntity.GetComponent<TransformComponent>().position;
            }
            
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief 处理鼠标释放
     * @param button 鼠标按钮
     * @return 如果处理了事件返回 true
     */
    bool HandleMouseRelease(int button) {
        if (button == 0 && m_isDragging) {
            m_isDragging = false;
            m_activeAxis = GizmoAxis::None;
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief 获取当前激活的轴向
     */
    GizmoAxis GetActiveAxis() const {
        return m_activeAxis;
    }
    
    /**
     * @brief 获取是否正在拖动
     */
    bool IsDragging() const {
        return m_isDragging;
    }
    
    /**
     * @brief 计算移动增量
     * @param deltaX 鼠标 X 增量
     * @param deltaY 鼠标 Y 增量
     * @param camera 相机对象
     * @return 世界空间移动向量
     */
    glm::vec3 CalculateMoveDelta(float deltaX, float deltaY, Camera* camera) {
        if (!camera) return glm::vec3(0);
        
        glm::vec3 delta(0);
        
        switch (m_activeAxis) {
            case GizmoAxis::X:
                delta.x = deltaX * 0.01f;
                break;
            case GizmoAxis::Y:
                delta.y = deltaY * 0.01f;
                break;
            case GizmoAxis::Z:
                delta.z = deltaX * 0.01f;
                break;
            case GizmoAxis::XY:
                delta.x = deltaX * 0.01f;
                delta.y = deltaY * 0.01f;
                break;
            default:
                break;
        }
        
        // 应用吸附
        if (m_snapEnabled && m_snapValue > 0) {
            delta.x = round(delta.x / m_snapValue) * m_snapValue;
            delta.y = round(delta.y / m_snapValue) * m_snapValue;
            delta.z = round(delta.z / m_snapValue) * m_snapValue;
        }
        
        return delta;
    }

private:
    GizmoType m_gizmoType;
    GizmoAxis m_activeAxis;
    GizmoAxis m_hoveredAxis;
    
    Entity m_targetEntity;
    glm::vec3 m_position{0};
    
    bool m_isDragging;
    float m_dragStartX, m_dragStartY;
    glm::vec3 m_dragStartValue;
    
    bool m_snapEnabled;
    float m_snapValue;
};

/**
 * @brief Gizmo 管理器
 * @details 管理场景中所有 Gizmo 的显示和操作
 */
class GizmoManager {
public:
    static GizmoManager& Instance() {
        static GizmoManager instance;
        return instance;
    }
    
    /**
     * @brief 设置当前 Gizmo 类型
     * @param type Gizmo 类型
     */
    void SetGizmoType(GizmoType type) {
        m_mainGizmo.SetType(type);
    }
    
    /**
     * @brief 获取当前 Gizmo 类型
     */
    GizmoType GetGizmoType() const {
        return m_mainGizmo.GetType();
    }
    
    /**
     * @brief 设置目标实体
     * @param entity 目标实体
     */
    void SetTarget(Entity entity) {
        m_mainGizmo.SetTarget(entity);
    }
    
    /**
     * @brief 获取主 Gizmo
     */
    Gizmo& GetMainGizmo() {
        return m_mainGizmo;
    }
    
    /**
     * @brief 启用/禁用吸附
     * @param enabled 是否启用
     */
    void SetSnapEnabled(bool enabled) {
        m_mainGizmo.SetSnapEnabled(enabled);
    }
    
    /**
     * @brief 设置吸附值
     * @param value 吸附值
     */
    void SetSnapValue(float value) {
        m_mainGizmo.SetSnapValue(value);
    }
    
    /**
     * @brief 更新所有 Gizmo
     */
    void Update(Camera* camera, float mouseX, float mouseY) {
        m_mainGizmo.Update(camera, mouseX, mouseY);
    }
    
    /**
     * @brief 渲染所有 Gizmo
     */
    void Render() {
        m_mainGizmo.Render();
    }

private:
    GizmoManager() = default;
    Gizmo m_mainGizmo;
};
