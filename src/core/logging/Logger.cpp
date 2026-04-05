#include "Logger.h"
#include <chrono>
#include <iomanip>

namespace Logging {

// 静态实例
Logger* g_loggerInstance = nullptr;

Logger& Logger::Instance() {
    if (!g_loggerInstance) {
        g_loggerInstance = new Logger();
    }
    return *g_loggerInstance;
}

Logger::Logger() {
}

Logger::~Logger() {
    if (m_fileStream && m_fileStream->is_open()) {
        m_fileStream->close();
    }
}

void Logger::Log(LogLevel level, const std::string& module, 
                 const std::string& message) {
    // 检查是否低于最小级别
    if (level < m_minimumLevel) {
        return;
    }
    
    // 构建日志消息
    std::ostringstream oss;
    oss << "[" << GetTimestamp() << "] "
        << "[" << GetLevelString(level) << "] "
        << "[" << module << "] "
        << message;
    
    std::string logMessage = oss.str();
    
    // 输出到控制台
    if (m_consoleOutput) {
        // 根据级别选择输出流
        if (level >= LogLevel::Warning) {
            std::cerr << logMessage << std::endl;
        } else {
            std::cout << logMessage << std::endl;
        }
    }
    
    // 输出到文件
    if (m_fileOutput && m_fileStream && m_fileStream->is_open()) {
        *m_fileStream << logMessage << std::endl;
        m_fileStream->flush();
    }
    
    // 更新统计信息
    m_stats.totalLogs++;
    if (level == LogLevel::Error) {
        m_stats.errorCount++;
    } else if (level == LogLevel::Warning) {
        m_stats.warningCount++;
    }
}

void Logger::SetOutputFile(const std::string& filepath) {
    // 关闭旧文件
    if (m_fileStream && m_fileStream->is_open()) {
        m_fileStream->close();
    }
    
    // 打开新文件
    m_fileStream = std::make_unique<std::ofstream>(
        filepath, 
        std::ios::app  // 追加模式
    );
    
    if (m_fileStream && m_fileStream->is_open()) {
        m_fileOutput = true;
        Log(LogLevel::Info, "Logger", "Log file opened: " + filepath);
    } else {
        m_fileOutput = false;
        std::cerr << "Failed to open log file: " << filepath << std::endl;
    }
}

void Logger::EnableConsoleOutput(bool enable) {
    m_consoleOutput = enable;
}

void Logger::EnableFileOutput(bool enable) {
    m_fileOutput = enable && m_fileStream && m_fileStream->is_open();
}

void Logger::SetMinimumLevel(LogLevel level) {
    m_minimumLevel = level;
}

void Logger::ClearStatistics() {
    m_stats = Statistics();
}

void Logger::Flush() {
    if (m_fileStream && m_fileStream->is_open()) {
        m_fileStream->flush();
    }
}

std::string Logger::GetLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    struct tm timeinfo;
    localtime_s(&timeinfo, &time);
    oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

}  // namespace Logging
