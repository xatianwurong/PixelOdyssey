#pragma once
#include "../core/ecs/Entity.h"
#include "../core/ecs/Component.h"
#include "../core/renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

/**
 * @brief Gizmo 类型枚举
 * @details 定义编辑器中变换 Gizmo 的类型
 */
enum class GizmoType
{
    None = 0,  ///< 无 Gizmo
    Move,      ///< 移动 Gizmo
    Rotate,    ///< 旋转 Gizmo
    Scale      ///< 缩放 Gizmo
};

/**
 * @brief Gizmo 轴向枚举
 * @details 定义 Gizmo 操作的空间轴向
 */
enum class GizmoAxis
{
    None = 0,  ///< 无轴向
    X,         ///< X 轴
    Y,         ///< Y 轴
    Z,         ///< Z 轴
    XY,        ///< XY 平面
    XZ,        ///< XZ 平面
    YZ,        ///< YZ 平面
    XYZ        ///< 三轴
};

/**
 * @brief Gizmo 类
 * @brief 渲染和操作变换 Gizmo（移动/旋转/缩放手柄）
 * @details 用于在编辑器中可视化并操作实体的变换
 * @details 支持移动、旋转、缩放三种模式
 * @details 提供轴向选择、网格吸附等功能
 * @功能描述 实现 3D 编辑器的交互式变换操作
 * @设计目的 提供直观的实体变换编辑能力
 * @使用场景 编辑器中选择并调整实体位置、旋转、缩放
 * @关键实现 支持轴向检测、鼠标交互、吸附功能
 */
class Gizmo
{
public:
    /**
     * @brief 构造函数
     */
    Gizmo()
        : m_gizmoType(GizmoType::None)
        , m_activeAxis(GizmoAxis::None)
        , m_hoveredAxis(GizmoAxis::None)
        , m_isDragging(false)
        , m_snapEnabled(false)
        , m_snapValue(1.0f) {}

    /**
     * @brief 设置 Gizmo 类型
     * @param type Gizmo 类型（Move/Rotate/Scale）
     */
    void SetType(GizmoType type)
    {
        m_gizmoType = type;
    }

    /**
     * @brief 获取 Gizmo 类型
     * @return 当前 Gizmo 类型
     */
    GizmoType GetType() const
    {
        return m_gizmoType;
    }

    /**
     * @brief 设置目标实体
     * @param entity 要操作的实体
     */
    void SetTarget(Entity entity)
    {
        m_targetEntity = entity;
    }

    /**
     * @brief 获取目标实体
     * @return 当前目标实体
     */
    Entity GetTarget() const
    {
        return m_targetEntity;
    }

    /**
     * @brief 启用/禁用网格吸附
     * @param enabled 是否启用吸附
     */
    void SetSnapEnabled(bool enabled)
    {
        m_snapEnabled = enabled;
    }

    /**
     * @brief 设置吸附值
     * @param value 吸附网格大小（如 1.0 表示 1 单位吸附）
     */
    void SetSnapValue(float value)
    {
        m_snapValue = value;
    }

    /**
     * @brief 更新 Gizmo 状态
     * @param camera 相机对象
     * @param mouseX 鼠标 X 坐标
     * @param mouseY 鼠标 Y 坐标
     * @details 每帧调用，更新 Gizmo 位置跟随目标实体
     */
    void Update(Camera* camera, float mouseX, float mouseY)
    {
        if (!m_targetEntity.IsValid()) return;

        // 更新 Gizmo 位置跟随目标实体
        if (m_targetEntity.HasComponent<TransformComponent>())
        {
            m_position = m_targetEntity.GetComponent<TransformComponent>().position;
        }
    }

    /**
     * @brief 渲染 Gizmo
     * @details 绘制 Gizmo 的可视化表示
     * @details 根据类型绘制不同的 Gizmo：
     * @details - Move: 三个轴向的箭头
     * @details - Rotate: 三个轴向的圆环
     * @details - Scale: 三个轴向的方块
     */
    void Render()
    {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None)
        {
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
     * @brief 处理鼠标移动事件
     * @param camera 相机对象
     * @param mouseX 鼠标 X 坐标
     * @param mouseY 鼠标 Y 坐标
     * @return 如果处理了事件返回 true
     * @details 检测鼠标是否悬停在 Gizmo 的某个轴上
     */
    bool HandleMouseMove(Camera* camera, float mouseX, float mouseY)
    {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None)
        {
            return false;
        }

        // TODO: 实现鼠标悬停检测
        // 检测鼠标是否悬停在 Gizmo 的某个轴上

        return false;
    }

    /**
     * @brief 处理鼠标按下事件
     * @param camera 相机对象
     * @param mouseX 鼠标 X 坐标
     * @param mouseY 鼠标 Y 坐标
     * @param button 鼠标按钮（0=左键，1=中键，2=右键）
     * @return 如果处理了事件返回 true
     * @details 检测点击了哪个轴，开始拖动操作
     */
    bool HandleMousePress(Camera* camera, float mouseX, float mouseY, int button)
    {
        if (!m_targetEntity.IsValid() || m_gizmoType == GizmoType::None)
        {
            return false;
        }

        if (button == 0)  // 左键
        {
            // TODO: 检测点击了哪个轴
            // 如果点击了轴，开始拖动

            m_isDragging = true;
            m_dragStartX = mouseX;
            m_dragStartY = mouseY;

            if (m_targetEntity.HasComponent<TransformComponent>())
            {
                m_dragStartValue = m_targetEntity.GetComponent<TransformComponent>().position;
            }

            return true;
        }

        return false;
    }

    /**
     * @brief 处理鼠标释放事件
     * @param button 鼠标按钮
     * @return 如果处理了事件返回 true
     * @details 结束拖动操作，重置状态
     */
    bool HandleMouseRelease(int button)
    {
        if (button == 0 && m_isDragging)
        {
            m_isDragging = false;
            m_activeAxis = GizmoAxis::None;
            return true;
        }

        return false;
    }

    /**
     * @brief 获取当前激活的轴向
     * @return 当前激活的轴向
     */
    GizmoAxis GetActiveAxis() const
    {
        return m_activeAxis;
    }

    /**
     * @brief 获取是否正在拖动
     * @return true 表示正在拖动
     */
    bool IsDragging() const
    {
        return m_isDragging;
    }

    /**
     * @brief 计算移动增量
     * @param deltaX 鼠标 X 增量
     * @param deltaY 鼠标 Y 增量
     * @param camera 相机对象
     * @return 世界空间移动向量
     * @details 根据激活的轴向和鼠标移动计算移动量
     * @details 支持吸附功能
     */
    glm::vec3 CalculateMoveDelta(float deltaX, float deltaY, Camera* camera)
    {
        if (!camera) return glm::vec3(0);

        glm::vec3 delta(0);

        switch (m_activeAxis)
        {
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
        if (m_snapEnabled && m_snapValue > 0)
        {
            delta.x = round(delta.x / m_snapValue) * m_snapValue;
            delta.y = round(delta.y / m_snapValue) * m_snapValue;
            delta.z = round(delta.z / m_snapValue) * m_snapValue;
        }

        return delta;
    }

private:
    GizmoType m_gizmoType;         ///< Gizmo 类型
    GizmoAxis m_activeAxis;        ///< 当前激活的轴向
    GizmoAxis m_hoveredAxis;       ///< 鼠标悬停的轴向

    Entity m_targetEntity;         ///< 目标实体
    glm::vec3 m_position{0};       ///< Gizmo 位置

    bool m_isDragging;             ///< 是否正在拖动
    float m_dragStartX, m_dragStartY;  ///< 拖动起始位置
    glm::vec3 m_dragStartValue;    ///< 拖动起始值

    bool m_snapEnabled;            ///< 是否启用吸附
    float m_snapValue;             ///< 吸附值
};

/**
 * @brief Gizmo 管理器类
 * @brief 管理场景中所有 Gizmo 的显示和操作
 * @details 单例模式，提供全局访问点
 * @details 管理主 Gizmo 的状态和行为
 * @功能描述 统一管理编辑器中的 Gizmo
 * @设计目的 提供集中化的 Gizmo 管理
 * @使用场景 编辑器中控制 Gizmo 的显示和操作
 * @关键实现 单例模式，委托给内部 Gizmo 对象
 */
class GizmoManager
{
public:
    /**
     * @brief 获取单例实例
     * @return GizmoManager 实例引用
     */
    static GizmoManager& Instance()
    {
        static GizmoManager instance;
        return instance;
    }

    /**
     * @brief 设置当前 Gizmo 类型
     * @param type Gizmo 类型
     */
    void SetGizmoType(GizmoType type)
    {
        m_mainGizmo.SetType(type);
    }

    /**
     * @brief 获取当前 Gizmo 类型
     * @return 当前 Gizmo 类型
     */
    GizmoType GetGizmoType() const
    {
        return m_mainGizmo.GetType();
    }

    /**
     * @brief 设置目标实体
     * @param entity 目标实体
     */
    void SetTarget(Entity entity)
    {
        m_mainGizmo.SetTarget(entity);
    }

    /**
     * @brief 获取主 Gizmo
     * @return 主 Gizmo 引用
     */
    Gizmo& GetMainGizmo()
    {
        return m_mainGizmo;
    }

    /**
     * @brief 启用/禁用吸附
     * @param enabled 是否启用
     */
    void SetSnapEnabled(bool enabled)
    {
        m_mainGizmo.SetSnapEnabled(enabled);
    }

    /**
     * @brief 设置吸附值
     * @param value 吸附值
     */
    void SetSnapValue(float value)
    {
        m_mainGizmo.SetSnapValue(value);
    }

    /**
     * @brief 更新所有 Gizmo
     * @param camera 相机对象
     * @param mouseX 鼠标 X 坐标
     * @param mouseY 鼠标 Y 坐标
     */
    void Update(Camera* camera, float mouseX, float mouseY)
    {
        m_mainGizmo.Update(camera, mouseX, mouseY);
    }

    /**
     * @brief 渲染所有 Gizmo
     */
    void Render()
    {
        m_mainGizmo.Render();
    }

private:
    GizmoManager() = default;  ///< 私有构造函数（单例模式）
    Gizmo m_mainGizmo;         ///< 主 Gizmo 对象
};
