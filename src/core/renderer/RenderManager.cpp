#include "RenderManager.h"
#include "../logging/Logger.h"
#include <algorithm>
#include <chrono>

using namespace Logging;

RenderManager::FrameTimer::FrameTimer() 
    : m_start(std::chrono::high_resolution_clock::now()) {
}

RenderManager::FrameTimer::~FrameTimer() {
}

float RenderManager::FrameTimer::GetElapsedMilliseconds() const {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end - m_start);
    return duration.count() / 1000.0f;
}

RenderManager::RenderManager(std::shared_ptr<IRenderContext> context)
    : m_context(context) {
    if (!context) {
        LOG_ERROR("RenderManager", "RenderContext is null");
        throw std::invalid_argument("RenderContext cannot be null");
    }
    LOG_INFO("RenderManager", "RenderManager initialized");
}

RenderManager::~RenderManager() {
    ClearAll();
    LOG_INFO("RenderManager", "RenderManager destroyed");
}

void RenderManager::AddObject(std::shared_ptr<IRenderable> object) {
    if (!object) {
        LOG_ERROR("RenderManager", "Cannot add null object");
        throw std::invalid_argument("Object cannot be null");
    }

    try {
        if (!object->LoadResources()) {
            LOG_ERROR("RenderManager", "Failed to load resources for object");
            return;
        }

        m_renderables.push_back(object);
        LOG_DEBUG("RenderManager", "Object added, total: " + 
                  std::to_string(m_renderables.size()));
    } catch (const std::exception& e) {
        LOG_ERROR("RenderManager", std::string("Exception adding object: ") + e.what());
        throw;
    }
}

void RenderManager::RemoveObject(IRenderable* object) {
    if (!object) {
        LOG_WARNING("RenderManager", "Attempting to remove null object");
        return;
    }

    auto it = std::find_if(m_renderables.begin(), m_renderables.end(),
        [object](const std::shared_ptr<IRenderable>& ptr) {
            return ptr.get() == object;
        });

    if (it != m_renderables.end()) {
        try {
            (*it)->OnDestroy();
            (*it)->UnloadResources();
            m_renderables.erase(it);
            LOG_DEBUG("RenderManager", "Object removed, total: " + 
                      std::to_string(m_renderables.size()));
        } catch (const std::exception& e) {
            LOG_ERROR("RenderManager", std::string("Exception removing object: ") + e.what());
        }
    }
}

void RenderManager::ClearAll() {
    try {
        for (auto& obj : m_renderables) {
            obj->OnDestroy();
            obj->UnloadResources();
        }
        m_renderables.clear();
        LOG_INFO("RenderManager", "All objects cleared");
    } catch (const std::exception& e) {
        LOG_ERROR("RenderManager", std::string("Exception clearing objects: ") + e.what());
    }
}

void RenderManager::Update(float deltaTime) {
    try {
        for (auto& obj : m_renderables) {
            obj->Update(deltaTime);
        }
    } catch (const std::exception& e) {
        LOG_ERROR("RenderManager", std::string("Exception during update: ") + e.what());
    }
}

void RenderManager::Render() {
    if (!m_context) {
        LOG_ERROR("RenderManager", "RenderContext is null");
        return;
    }

    FrameTimer timer;

    try {
        // 清空统计信息
        m_stats.visibleObjectCount = 0;
        m_stats.totalObjectCount = m_renderables.size();
        m_stats.totalDrawCalls = 0;

        // 排序对象
        SortRenderables();

        // 渲染所有对象
        for (auto& obj : m_renderables) {
            if (obj->IsVisible()) {
                m_stats.visibleObjectCount++;
                obj->Render(m_context.get());
                m_stats.totalDrawCalls++;
            }
        }

        if (m_profilingEnabled) {
            m_stats.lastFrameTime = timer.GetElapsedMilliseconds();
            LOG_DEBUG("RenderManager", "Frame time: " + 
                      std::to_string(m_stats.lastFrameTime) + " ms");
        }
    } catch (const std::exception& e) {
        LOG_ERROR("RenderManager", std::string("Exception during render: ") + e.what());
    }
}

void RenderManager::SortRenderables() {
    // 按渲染层级排序（低层级先渲染）
    std::stable_sort(m_renderables.begin(), m_renderables.end(),
        [](const std::shared_ptr<IRenderable>& a, 
           const std::shared_ptr<IRenderable>& b) {
            return a->GetRenderLayer() < b->GetRenderLayer();
        });
}
