#pragma once

#include "interfaces/IRenderable.h"
#include "interfaces/IRenderContext.h"
#include <vector>
#include <memory>
#include <chrono>

/**
 * @brief 渲染管理器类
 * @details 管理场景中的可渲染对象
 * @details 协调渲染流程和更新周期
 * @details 提供性能统计和调试信息
 * @details 使用依赖注入而非单例模式
 * @功能描述 管理渲染对象的生命周期和渲染流程
 * @设计目的 将对象管理与 GPU 命令分离，支持分层渲染和优化
 * @使用场景 3D 场景渲染管理
 * @关键实现 按渲染层级排序，支持性能分析
 */
class RenderManager
{
public:
  /**
   * @brief 构造函数
   * @param context 渲染上下文智能指针（必需）
   */
  explicit RenderManager(std::shared_ptr<IRenderContext> context);

  /**
   * @brief 析构函数
   * @details 自动清理所有对象
   */
  ~RenderManager();

  // ========== 对象管理 ==========

  /**
   * @brief 添加可渲染对象
   * @param object 可渲染对象智能指针，不能为 null
   * @throws std::invalid_argument 如果 object 为 null
   * @details 对象生命周期由 RenderManager 管理，自动调用 LoadResources()
   */
  void AddObject(std::shared_ptr<IRenderable> object);

  /**
   * @brief 移除可渲染对象
   * @param object 可渲染对象指针
   * @details 自动调用 UnloadResources() 和 OnDestroy()
   */
  void RemoveObject(IRenderable* object);

  /**
   * @brief 清除所有对象
   * @details 移除所有已添加的可渲染对象
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
   * @details 应在每帧开始时调用
   */
  void Update(float deltaTime);

  /**
   * @brief 渲染所有对象
   * @details 按层级排序，跳过不可见对象，应在每帧调用 Update 后调用
   */
  void Render();

  // ========== 调试信息 ==========

  /**
   * @brief 渲染统计信息结构体
   */
  struct RenderStats
  {
    uint32_t visibleObjectCount = 0;    ///< 本帧显示的对象数
    uint32_t totalObjectCount = 0;      ///< 总对象数
    uint32_t totalDrawCalls = 0;        ///< 总绘制调用数
    float lastFrameTime = 0.0f;         ///< 上一帧耗时（毫秒）
  };

  /**
   * @brief 获取渲染统计信息
   * @return 统计信息常量引用
   */
  const RenderStats& GetStats() const { return m_stats; }

  /**
   * @brief 启用性能分析
   * @param enable true 启用性能计时，false 禁用
   */
  void SetProfilingEnabled(bool enable) { m_profilingEnabled = enable; }

private:
  std::shared_ptr<IRenderContext> m_context;                      ///< 渲染上下文
  std::vector<std::shared_ptr<IRenderable>> m_renderables;        ///< 可渲染对象列表
  RenderStats m_stats;                                            ///< 渲染统计信息
  bool m_profilingEnabled = false;                                ///< 是否启用性能分析

  /**
   * @brief 按渲染层级排序对象
   * @details 确保正确的渲染顺序
   */
  void SortRenderables();

  /**
   * @brief 帧计时器类
   * @details 用于测量渲染帧的耗时
   */
  class FrameTimer
  {
  public:
    /**
     * @brief 构造函数
     * @details 记录开始时间
     */
    FrameTimer();

    /**
     * @brief 析构函数
     */
    ~FrameTimer();

    /**
     * @brief 获取经过的时间
     * @return 从构造到现在的时间（毫秒）
     */
    float GetElapsedMilliseconds() const;

  private:
    std::chrono::high_resolution_clock::time_point m_start;  ///< 开始时间点
  };
};