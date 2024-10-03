#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

class Logger {
 public:
  // 日志级别枚举
  enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

  // 获取 Logger 单例实例
  static Logger& getInstance() {
    static Logger instance;
    return instance;
  }

  // 禁止拷贝和赋值
  Logger(const Logger&)            = delete;
  Logger& operator=(const Logger&) = delete;

  // 设置日志文件，基于当前时间生成唯一文件名
  void initLogFile() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string fileName = generateLogFileName();
    logFile_.open(fileName, std::ios::app);
    if (!logFile_.is_open()) {
      std::cerr << "Failed to open log file: " << fileName << std::endl;
    }
  }

  // 输出日志到控制台和文件
  void log(LogLevel level, const std::string& message,
           const std::string& functionName) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string logMessage = formatLogMessage(level, message, functionName);

    // 输出到控制台
    std::cout << logMessage << std::endl;

    // 写入日志文件
    if (logFile_.is_open()) {
      logFile_ << logMessage << std::endl;
    }
  }

  // 各级别日志的快捷方法
  void debug(const std::string& message, const std::string& functionName) {
    log(LogLevel::DEBUG, message, functionName);
  }
  void info(const std::string& message, const std::string& functionName) {
    log(LogLevel::INFO, message, functionName);
  }
  void warn(const std::string& message, const std::string& functionName) {
    log(LogLevel::WARNING, message, functionName);
  }
  void error(const std::string& message, const std::string& functionName) {
    log(LogLevel::ERROR, message, functionName);
  }

 private:
  Logger() = default;
  ~Logger() {
    if (logFile_.is_open()) {
      logFile_.close();
    }
  }

  // 生成基于日期时间的日志文件名
  static std::string generateLogFileName() {
    const std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H-%M-%S",
                  std::localtime(&now));
    return "log_" + std::string(timeStr) + ".log";
  }

  // 格式化日志消息，包含时间、线程ID、日志级别和函数名
  static std::string formatLogMessage(LogLevel level,
                                      const std::string& message,
                                      const std::string& functionName) {
    std::string levelStr;
    switch (level) {
      case LogLevel::DEBUG:
        levelStr = "DEBUG";
        break;
      case LogLevel::INFO:
        levelStr = "INFO";
        break;
      case LogLevel::WARNING:
        levelStr = "WARNING";
        break;
      case LogLevel::ERROR:
        levelStr = "ERROR";
        break;
    }

    // 获取当前时间
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&now));

    // 获取当前线程ID
    std::ostringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    std::string threadIdStr = threadIdStream.str();

    return "[" + std::string(timeStr) + "] [Thread " + threadIdStr + "] [" +
           levelStr + "] [" + functionName + "] " + message;
  }

  std::ofstream logFile_;  // 日志文件输出流
  std::mutex mutex_;       // 线程安全的互斥锁
};

// 辅助宏，用于简化函数名传递
#define LOG_DEBUG(message) Logger::getInstance().debug(message, __FUNCTION__)
#define LOG_INFO(message) Logger::getInstance().info(message, __FUNCTION__)
#define LOG_WARN(message) Logger::getInstance().warn(message, __FUNCTION__)
#define LOG_ERROR(message) Logger::getInstance().error(message, __FUNCTION__)

int main() {
  // 初始化日志文件
  Logger::getInstance().initLogFile();

  // 使用示例
  LOG_DEBUG("This is a debug message.");
  LOG_INFO("This is an info message.");
  LOG_WARN("This is a warning message.");
  LOG_ERROR("This is an error message.");

  return 0;
}
