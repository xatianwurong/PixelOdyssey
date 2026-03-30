#pragma once
#include <chrono>
#include <string>
#include <deque>

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