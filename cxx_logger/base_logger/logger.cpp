#include "logger.h"
#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

Logger::Logger() : logLevel_(LogLevel::INFO) {}

Logger::~Logger() {
  if (logFile_.is_open()) {
    logFile_.close();
  }
}

// 设置日志级别
void Logger::setLogLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(mutex_);
  logLevel_ = level;
}

// 设置日志文件路径
void Logger::setLogFile(const std::string& filename) {
  std::lock_guard<std::mutex> lock(mutex_);
  logFile_.open(filename, std::ios::app);
  if (!logFile_) {
    std::cerr << "Unable to open log file: " << filename << std::endl;
  }
}

// 日志输出函数
void Logger::log(LogLevel level, const std::string& message,
                 const char* funcName)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (level >= logLevel_) {
    std::ostringstream oss;
    oss << "[" << getCurrentTime() << " - " << logLevelToString(level) << " - "
        << std::this_thread::get_id() << " - " << funcName << "]: " << message
        << std::endl;

    std::string logEntry = oss.str();
    std::cout << logEntry;  // 打印到控制台
    if (logFile_.is_open()) {
      logFile_ << logEntry;  // 写入日志文件
    }
  }
}

// 将日志级别转换为字符串
std::string Logger::logLevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::DEBUG:
      return "DEBUG";
    default:
      return "UNKNOWN";
  }
}

// 获取当前时间字符串
std::string Logger::getCurrentTime() {
  auto now   = std::chrono::system_clock::now();
  auto timeT = std::chrono::system_clock::to_time_t(now);
  auto ms    = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S") << '.'
      << std::setw(3) << std::setfill('0') << ms.count();
  return oss.str();
}
