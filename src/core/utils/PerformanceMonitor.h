#pragma once
#include <chrono>
#include <string>
#include <deque>

/**
 * @class PerformanceMonitor
 * @brief 性能监控类
 * @details 监控应用程序的性能，包括帧率、帧时间和 delta 时间
 * @功能描述 提供性能监控和统计功能
 * @设计目的 使用单例模式，确保全局性能监控
 * @使用场景 用于监控应用程序的运行性能
 * @关键实现 使用单例模式，通过Instance()方法获取实例
 */
class PerformanceMonitor
{
public:
    static PerformanceMonitor& Instance();
    ~PerformanceMonitor() = default;

    void BeginFrame();
    void EndFrame();

    float GetFPS() const { return m_fps; }
    float GetFrameTime() const { return m_frameTime; }
    float GetDeltaTime() const { return m_deltaTime; }

private:
    PerformanceMonitor() = default;

    std::chrono::high_resolution_clock::time_point m_frameStart;
    std::chrono::high_resolution_clock::time_point m_frameEnd;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;

    float m_fps = 0.0f;
    float m_frameTime = 0.0f;
    float m_deltaTime = 0.0f;
    int m_frameCount = 0;
    float m_fpsTimer = 0.0f;
};