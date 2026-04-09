#include "RenderManager.h"
#include "../logging/Logger.h"
#include <algorithm>
#include <chrono>

using namespace Logging;

/**
 * @brief 帧计时器内部类
 * @details 用于测量渲染帧的时间
 */
RenderManager::FrameTimer::FrameTimer()
    : m_start(std::chrono::high_resolution_clock::now())
{
}

RenderManager::FrameTimer::~FrameTimer()
{
}

/**
 * @brief 获取经过的时间
 * @return 毫秒数
 * @details 从计时器创建到现在的时间
 */
float RenderManager::FrameTimer::GetElapsedMilliseconds() const
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end - m_start);
    return duration.count() / 1000.0f;
}

/**
 * @brief 构造函数
 * @param context 渲染上下文对象
 * @details 初始化渲染管理器并创建渲染上下文
 */
RenderManager::RenderManager(std::shared_ptr<IRenderContext> context)
    : m_context(context)
{
    if (!context)
    {
        LOG_ERROR("RenderManager", "RenderContext is null");
        throw std::invalid_argument("RenderContext cannot be null");
    }
    LOG_INFO("RenderManager", "RenderManager initialized");
}

/**
 * @brief 析构函数
 * @details 清理所有渲染对象
 */
RenderManager::~RenderManager()
{
    ClearAll();
    LOG_INFO("RenderManager", "RenderManager destroyed");
}

/**
 * @brief 添加渲染对象
 * @param object 要添加的渲染对象
 * @details 将对象添加到渲染列表
 * @details 自动调用对象的 LoadResources 方法
 */
void RenderManager::AddObject(std::shared_ptr<IRenderable> object)
{
    if (!object)
    {
        LOG_ERROR("RenderManager", "Cannot add null object");
        throw std::invalid_argument("Object cannot be null");
    }

    try
    {
        if (!object->LoadResources())
        {
            LOG_ERROR("RenderManager", "Failed to load resources for object");
            return;
        }

        m_renderables.push_back(object);
        LOG_DEBUG("RenderManager", "Object added, total: " +
                  std::to_string(m_renderables.size()));
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("RenderManager", std::string("Exception adding object: ") + e.what());
        throw;
    }
}

/**
 * @brief 移除渲染对象
 * @param object 要移除的渲染对象指针
 * @details 从渲染列表中移除对象
 * @details 自动调用对象的 OnDestroy 和 UnloadResources 方法
 */
void RenderManager::RemoveObject(IRenderable* object)
{
    if (!object)
    {
        LOG_WARNING("RenderManager", "Attempting to remove null object");
        return;
    }

    auto it = std::find_if(m_renderables.begin(), m_renderables.end(),
        [object](const std::shared_ptr<IRenderable>& ptr)
        {
            return ptr.get() == object;
        });

    if (it != m_renderables.end())
    {
        try
        {
            (*it)->OnDestroy();
            (*it)->UnloadResources();
            m_renderables.erase(it);
            LOG_DEBUG("RenderManager", "Object removed, total: " +
                      std::to_string(m_renderables.size()));
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("RenderManager", std::string("Exception removing object: ") + e.what());
        }
    }
}

/**
 * @brief 清空所有渲染对象
 * @details 移除并清理所有渲染对象
 * @details 调用每个对象的 OnDestroy 和 UnloadResources 方法
 */
void RenderManager::ClearAll()
{
    try
    {
        for (auto& obj : m_renderables)
        {
            obj->OnDestroy();
            obj->UnloadResources();
        }
        m_renderables.clear();
        LOG_INFO("RenderManager", "All objects cleared");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("RenderManager", std::string("Exception clearing objects: ") + e.what());
    }
}

/**
 * @brief 更新所有渲染对象
 * @param deltaTime 帧间隔时间（秒）
 * @details 调用每个对象的 Update 方法
 */
void RenderManager::Update(float deltaTime)
{
    try
    {
        for (auto& obj : m_renderables)
        {
            obj->Update(deltaTime);
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("RenderManager", std::string("Exception during update: ") + e.what());
    }
}

/**
 * @brief 渲染所有对象
 * @details 执行渲染流程
 * @details 包括排序、渲染、性能统计
 */
void RenderManager::Render()
{
    if (!m_context)
    {
        LOG_ERROR("RenderManager", "RenderContext is null");
        return;
    }

    FrameTimer timer;

    try
    {
        // 清空统计信息
        m_stats.visibleObjectCount = 0;
        m_stats.totalObjectCount = m_renderables.size();
        m_stats.totalDrawCalls = 0;

        // 排序对象
        SortRenderables();

        // 渲染所有对象
        for (auto& obj : m_renderables)
        {
            if (obj->IsVisible())
            {
                m_stats.visibleObjectCount++;
                obj->Render(m_context.get());
                m_stats.totalDrawCalls++;
            }
        }

        if (m_profilingEnabled)
        {
            m_stats.lastFrameTime = timer.GetElapsedMilliseconds();
            LOG_DEBUG("RenderManager", "Frame time: " +
                      std::to_string(m_stats.lastFrameTime) + " ms");
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("RenderManager", std::string("Exception during render: ") + e.what());
    }
}

/**
 * @brief 对渲染对象进行排序
 * @details 按渲染层级排序（低层级先渲染）
 * @details 使用稳定排序保持相对顺序
 */
void RenderManager::SortRenderables()
{
    // 按渲染层级排序（低层级先渲染）
    std::stable_sort(m_renderables.begin(), m_renderables.end(),
        [](const std::shared_ptr<IRenderable>& a,
           const std::shared_ptr<IRenderable>& b)
        {
            return a->GetRenderLayer() < b->GetRenderLayer();
        });
}
