#include "PerformanceMonitor.h"
#include <iostream>

PerformanceMonitor& PerformanceMonitor::Instance()
{
    static PerformanceMonitor instance;
    return instance;
}

void PerformanceMonitor::BeginFrame()
{
    m_frameStart = std::chrono::high_resolution_clock::now();
}

void PerformanceMonitor::EndFrame()
{
    m_frameEnd = std::chrono::high_resolution_clock::now();
    
    auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(m_frameEnd - m_frameStart);
    m_frameTime = frameDuration.count() / 1000.0f;
    
    if (m_frameCount > 0)
    {
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_frameEnd - m_lastFrameTime);
        m_deltaTime = deltaTime.count() / 1000000.0f;
    }
    else
    {
        m_deltaTime = 0.016f;
    }
    
    m_lastFrameTime = m_frameEnd;
    m_fpsTimer += m_deltaTime;
    m_frameCount++;
    
    if (m_fpsTimer >= 1.0f)
    {
        m_fps = m_frameCount / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
        
        std::cout << "FPS: " << m_fps << ", Frame Time: " << m_frameTime << "ms" << std::endl;
    }
}