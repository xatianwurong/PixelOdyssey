#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Logging {

/**
 * @enum LogLevel
 * @brief 日志级别
 */
enum class LogLevel {
    Debug = 0,      ///< 调试信息
    Info = 1,       ///< 一般信息
    Warning = 2,    ///< 警告信息
    Error = 3,      ///< 错误信息
    Critical = 4    ///< 严重错误
};

/**
 * @class Logger
 * @brief 中央日志系统（单例模式）
 * @details
 *   - 提供统一的日志记录接口
 *   - 支持文件和控制台输出
 *   - 自动添加时间戳和日志级别
 * 
 * @usage
 *   auto& logger = Logger::Instance();
 *   logger.Log(LogLevel::Info, "Renderer", "Initializing render system");
 *   logger.SetOutputFile("app.log");
 */
class Logger {
public:
    /**
     * @brief 获取日志系统单例实例
     * @return Logger 引用
     */
    static Logger& Instance();
    
    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param module 模块名（如 "Renderer"、"UI"）
     * @param message 日志信息
     */
    void Log(LogLevel level, const std::string& module, 
             const std::string& message);
    
    /**
     * @brief 设置输出文件
     * @param filepath 文件路径
     * @note 如果文件已打开，旧文件将被关闭
     */
    void SetOutputFile(const std::string& filepath);
    
    /**
     * @brief 启用/禁用控制台输出
     * @param enable true 启用，false 禁用
     */
    void EnableConsoleOutput(bool enable);
    
    /**
     * @brief 启用/禁用文件输出
     * @param enable true 启用，false 禁用
     */
    void EnableFileOutput(bool enable);
    
    /**
     * @brief 设置最小日志级别
     * @param level 级别低于此值的日志将被忽略
     * @note 默认为 LogLevel::Debug（记录所有）
     */
    void SetMinimumLevel(LogLevel level);
    
    /**
     * @brief 获取日志统计信息
     * @return 返回结构体包含：总日志数、错误数、警告数
     */
    struct Statistics {
        uint32_t totalLogs = 0;
        uint32_t errorCount = 0;
        uint32_t warningCount = 0;
    };
    const Statistics& GetStatistics() const { return m_stats; }
    
    /**
     * @brief 清除日志统计
     */
    void ClearStatistics();
    
    /**
     * @brief 刷新输出缓冲区
     * @note 确保所有日志被写入文件
     */
    void Flush();
    
private:
    Logger();
    ~Logger();
    
    // 禁止拷贝
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::unique_ptr<std::ofstream> m_fileStream;
    bool m_consoleOutput = true;
    bool m_fileOutput = false;
    LogLevel m_minimumLevel = LogLevel::Debug;
    Statistics m_stats;
    
    std::string GetLevelString(LogLevel level) const;
    std::string GetTimestamp() const;
};

}  // namespace Logging

// ========== 便利宏 ==========

/**
 * @def LOG_DEBUG
 * @brief 记录调试信息
 * @param module 模块名
 * @param msg 消息
 */
#define LOG_DEBUG(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Debug, module, msg)

/**
 * @def LOG_INFO
 * @brief 记录信息
 * @param module 模块名
 * @param msg 消息
 */
#define LOG_INFO(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Info, module, msg)

/**
 * @def LOG_WARNING
 * @brief 记录警告
 * @param module 模块名
 * @param msg 消息
 */
#define LOG_WARNING(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Warning, module, msg)

/**
 * @def LOG_ERROR
 * @brief 记录错误
 * @param module 模块名
 * @param msg 消息
 */
#define LOG_ERROR(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Error, module, msg)

/**
 * @def LOG_CRITICAL
 * @brief 记录严重错误
 * @param module 模块名
 * @param msg 消息
 */
#define LOG_CRITICAL(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Critical, module, msg)
