#pragma once

#include "interfaces/IRenderable.h"
#include "interfaces/IRenderContext.h"
#include <vector>
#include <memory>
#include <chrono>

/**
 * @class RenderManager
 * @brief 渲染管理器（重构版本）
 * @details
 *   - 管理场景中的可渲染对象
 *   - 协调渲染流程和更新周期
 *   - 提供性能统计和调试信息
 * 
 * @design
 *   - 使用依赖注入而非单例模式
 *   - 将对象管理与GPU命令分离
 *   - 支持分层渲染和优化
 * 
 * @usage
 *   auto renderCtx = std::make_shared<RenderContext>();
 *   auto manager = std::make_unique<RenderManager>(renderCtx);
 *   manager->AddObject(myObject);
 *   manager->Update(deltaTime);
 *   manager->Render();
 */
class RenderManager {
public:
    /**
     * @brief 构造函数
     * @param context 渲染上下文（必需）
     */
    explicit RenderManager(std::shared_ptr<IRenderContext> context);

    /**
     * @brief 析构函数
     * @note 自动清理所有对象
     */
    ~RenderManager();

    // ========== 对象管理 ==========

    /**
     * @brief 添加可渲染对象
     * @param object 对象指针，不能为null
     * @throws std::invalid_argument 如果object为null
     * 
     * @note
     *   - 对象生命周期由RenderManager管理
     *   - 自动调用LoadResources()
     */
    void AddObject(std::shared_ptr<IRenderable> object);

    /**
     * @brief 移除可渲染对象
     * @param object 对象指针
     * @note 自动调用UnloadResources()和OnDestroy()
     */
    void RemoveObject(IRenderable* object);

    /**
     * @brief 清除所有对象
     */
    void ClearAll();

    /**
     * @brief 获取对象数量
     * @return 当前管理的对象总数
     */
    size_t GetObjectCount() const { return m_renderables.size(); }

    // ========== 渲染流程 ==========

    /**
     * @brief 更新所有对象
     * @param deltaTime 上一帧到现在的时间（秒）
     * @note 应在每帧开始时调用
     */
    void Update(float deltaTime);

    /**
     * @brief 渲染所有对象
     * @note
     *   - 按层级排序
     *   - 跳过不可见对象
     *   - 应在每帧调用Update后调用
     */
    void Render();

    // ========== 调试信息 ==========

    /**
     * @brief 渲染统计信息
     */
    struct RenderStats {
        uint32_t visibleObjectCount = 0;    ///< 本帧显示的对象数
        uint32_t totalObjectCount = 0;      ///< 总对象数
        uint32_t totalDrawCalls = 0;        ///< 总绘制调用数
        float lastFrameTime = 0.0f;         ///< 上一帧耗时（毫秒）
    };

    /**
     * @brief 获取渲染统计信息
     * @return 常引用
     */
    const RenderStats& GetStats() const { return m_stats; }

    /**
     * @brief 启用性能分析
     * @param enable true 启用性能计时
     */
    void SetProfilingEnabled(bool enable) { m_profilingEnabled = enable; }

private:
    std::shared_ptr<IRenderContext> m_context;
    std::vector<std::shared_ptr<IRenderable>> m_renderables;
    RenderStats m_stats;
    bool m_profilingEnabled = false;

    /**
     * @brief 按渲染层级排序对象
     */
    void SortRenderables();

    /**
     * @brief 帧计时
     */
    class FrameTimer {
    public:
        FrameTimer();
        ~FrameTimer();
        float GetElapsedMilliseconds() const;
    private:
        std::chrono::high_resolution_clock::time_point m_start;
    };
};