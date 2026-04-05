#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Logging {

/**
 * @brief 日志级别枚举
 * @details 定义不同严重程度的日志级别
 */
enum class LogLevel
{
  Debug = 0,      ///< 调试信息
  Info = 1,       ///< 一般信息
  Warning = 2,    ///< 警告信息
  Error = 3,      ///< 错误信息
  Critical = 4    ///< 严重错误
};

/**
 * @brief 日志记录器类
 * @brief 中央日志系统（单例模式）
 * @details 提供统一的日志记录接口
 * @details 支持文件和控制台输出
 * @details 自动添加时间戳和日志级别
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 记录和管理应用程序日志
 * @设计目的 提供统一的日志管理，便于调试和问题追踪
 * @使用场景 应用程序各模块的日志记录
 * @关键实现 单例模式，支持多输出目标和日志级别过滤
 */
class Logger
{
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
   * @details 如果文件已打开，旧文件将被关闭
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
   * @details 默认为 LogLevel::Debug（记录所有）
   */
  void SetMinimumLevel(LogLevel level);

  /**
   * @brief 日志统计信息结构体
   */
  struct Statistics
  {
    uint32_t totalLogs = 0;     ///< 总日志数
    uint32_t errorCount = 0;    ///< 错误数
    uint32_t warningCount = 0;  ///< 警告数
  };

  /**
   * @brief 获取日志统计信息
   * @return 统计信息常量引用
   */
  const Statistics& GetStatistics() const { return m_stats; }

  /**
   * @brief 清除日志统计
   * @details 重置所有计数器
   */
  void ClearStatistics();

  /**
   * @brief 刷新输出缓冲区
   * @details 确保所有日志被写入文件
   */
  void Flush();

private:
  /**
   * @brief 私有构造函数
   */
  Logger();

  /**
   * @brief 私有析构函数
   */
  ~Logger();

  /**
   * @brief 禁用拷贝构造函数
   */
  Logger(const Logger&) = delete;

  /**
   * @brief 禁用赋值运算符
   */
  Logger& operator=(const Logger&) = delete;

  std::unique_ptr<std::ofstream> m_fileStream;  ///< 文件输出流
  bool m_consoleOutput = true;                   ///< 是否启用控制台输出
  bool m_fileOutput = false;                     ///< 是否启用文件输出
  LogLevel m_minimumLevel = LogLevel::Debug;     ///< 最小日志级别
  Statistics m_stats;                            ///< 日志统计信息

  std::string GetLevelString(LogLevel level) const;  ///< 日志级别转字符串
  std::string GetTimestamp() const;                   ///< 获取时间戳
};

}  // namespace Logging

// ========== 便利宏 ==========

/**
 * @def LOG_DEBUG
 * @brief 记录调试信息
 * @param module 模块名
 * @param msg 消息内容
 */
#define LOG_DEBUG(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Debug, module, msg)

/**
 * @def LOG_INFO
 * @brief 记录信息
 * @param module 模块名
 * @param msg 消息内容
 */
#define LOG_INFO(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Info, module, msg)

/**
 * @def LOG_WARNING
 * @brief 记录警告
 * @param module 模块名
 * @param msg 消息内容
 */
#define LOG_WARNING(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Warning, module, msg)

/**
 * @def LOG_ERROR
 * @brief 记录错误
 * @param module 模块名
 * @param msg 消息内容
 */
#define LOG_ERROR(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Error, module, msg)

/**
 * @def LOG_CRITICAL
 * @brief 记录严重错误
 * @param module 模块名
 * @param msg 消息内容
 */
#define LOG_CRITICAL(module, msg) \
    Logging::Logger::Instance().Log(Logging::LogLevel::Critical, module, msg)
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
