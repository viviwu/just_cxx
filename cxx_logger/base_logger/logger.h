#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

// 日志级别枚举
enum class LogLevel { INFO, WARNING, DEBUG, ERROR };

class Logger {
 public:
  // 获取单例实例
  static Logger& getInstance() {
    static Logger instance;
    return instance;
  }

  // 设置日志级别
  void setLogLevel(LogLevel level);

  // 设置日志文件路径
  void setLogFile(const std::string& filename);

  // 日志输出函数
  void log(LogLevel level, const std::string& message, const char* funcName);

 private:
  Logger();

  ~Logger();

  // 禁用拷贝构造和赋值操作
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // 将日志级别转换为字符串
  std::string logLevelToString(LogLevel level);

  // 获取当前时间字符串
  std::string getCurrentTime();

  LogLevel logLevel_;
  std::ofstream logFile_;
  std::mutex mutex_;
};

// 日志宏定义便于使用
#define LOG(level, message) \
  Logger::getInstance().log(level, message, __FUNCTION__)

// 针对不同日志级别的简化宏定义
#define LOG_DEBUG(message) LOG(LogLevel::DEBUG, message)
#define LOG_INFO(message) LOG(LogLevel::INFO, message)
#define LOG_WARNING(message) LOG(LogLevel::WARNING, message)
#define LOG_ERROR(message) LOG(LogLevel::ERROR, message)

#endif  // LOGGER_H
