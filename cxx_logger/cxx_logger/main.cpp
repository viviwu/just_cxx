#include <atomic>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
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
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // 设置日志文件，基于当前时间生成唯一文件名
  void initLogFile(std::size_t maxFileSize = 10 * 1024 * 1024) {
    std::lock_guard<std::mutex> lock(mutex_);
    maxFileSize_     = maxFileSize;
    currentFileSize_ = 0;
    createNewLogFile();
  }

  // 设置日志级别
  void setLogLevel(LogLevel level) { minLogLevel_ = level; }

  // 设置日志格式，支持占位符
  void setLogFormat(const std::string& format) { logFormat_ = format; }

  // 输出日志（惰性生成消息）
  template <typename Func>
  void log(LogLevel level, Func messageGenerator,
           const std::string& functionName) {
    if (level < minLogLevel_)
      return;  // 过滤低于当前设置的日志级别

    // 惰性生成日志消息
    std::string message = messageGenerator();

    // 将日志消息加入队列，异步处理
    {
      std::lock_guard<std::mutex> lock(queueMutex_);
      logQueue_.emplace(formatLogMessage(level, message, functionName));
    }
    queueCondition_.notify_one();
  }

  // 各级别日志的快捷方法
  template <typename Func>
  void debug(Func messageGenerator, const std::string& functionName) {
    log(LogLevel::DEBUG, messageGenerator, functionName);
  }
  template <typename Func>
  void info(Func messageGenerator, const std::string& functionName) {
    log(LogLevel::INFO, messageGenerator, functionName);
  }
  template <typename Func>
  void warn(Func messageGenerator, const std::string& functionName) {
    log(LogLevel::WARNING, messageGenerator, functionName);
  }
  template <typename Func>
  void error(Func messageGenerator, const std::string& functionName) {
    log(LogLevel::ERROR, messageGenerator, functionName);
  }

 private:
  Logger() : minLogLevel_(LogLevel::DEBUG), stopLogging_(false) {
    // 启动后台线程处理日志
    logThread_ = std::thread(&Logger::logWorker, this);
  }

  ~Logger() {
    // 停止日志线程
    {
      std::lock_guard<std::mutex> lock(queueMutex_);
      stopLogging_ = true;
    }
    queueCondition_.notify_all();
    if (logThread_.joinable()) {
      logThread_.join();
    }

    // 关闭文件
    if (logFile_.is_open()) {
      logFile_.close();
    }
  }

  // 创建新的日志文件
  void createNewLogFile() {
    if (logFile_.is_open()) {
      logFile_.close();
    }
    std::string fileName = generateLogFileName();
    logFile_.open(fileName, std::ios::out);
    if (!logFile_.is_open()) {
      std::cerr << "Failed to open log file: " << fileName << std::endl;
    }
    currentFileSize_ = 0;
  }

  // 生成基于日期时间的日志文件名
  std::string generateLogFileName() {
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H-%M-%S",
                  std::localtime(&now));
    return "log_" + std::string(timeStr) + ".log";
  }

  // 格式化日志消息，包含时间、线程ID、日志级别和函数名，支持自定义格式
  std::string formatLogMessage(LogLevel level, const std::string& message,
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

    // 使用自定义格式
    std::string logMessage = logFormat_;
    logMessage = replacePlaceholder(logMessage, "{timestamp}", timeStr);
    logMessage = replacePlaceholder(logMessage, "{thread_id}", threadIdStr);
    logMessage = replacePlaceholder(logMessage, "{level}", levelStr);
    logMessage = replacePlaceholder(logMessage, "{function}", functionName);
    logMessage = replacePlaceholder(logMessage, "{message}", message);

    return logMessage;
  }

  // 替换日志格式中的占位符
  std::string replacePlaceholder(const std::string& format,
                                 const std::string& placeholder,
                                 const std::string& value) {
    std::string result = format;
    size_t pos         = result.find(placeholder);
    while (pos != std::string::npos) {
      result.replace(pos, placeholder.length(), value);
      pos = result.find(placeholder, pos + value.length());
    }
    return result;
  }

  // 日志处理后台线程
  void logWorker() {
    while (true) {
      std::unique_lock<std::mutex> lock(queueMutex_);
      queueCondition_.wait(lock,
                           [&] { return !logQueue_.empty() || stopLogging_; });

      if (stopLogging_ && logQueue_.empty()) {
        break;
      }

      while (!logQueue_.empty()) {
        std::string logMessage = logQueue_.front();
        logQueue_.pop();

        // 写入文件
        if (logFile_.is_open()) {
          logFile_ << logMessage << std::endl;
          currentFileSize_ += logMessage.size() + 1;

          if (currentFileSize_ >= maxFileSize_) {
            createNewLogFile();  // 文件滚动
          }
        }
      }
    }
  }

  std::ofstream logFile_;                   // 日志文件输出流
  std::mutex mutex_;                        // 日志文件互斥锁
  std::mutex queueMutex_;                   // 日志队列互斥锁
  std::condition_variable queueCondition_;  // 日志队列条件变量
  std::queue<std::string> logQueue_;        // 日志消息队列
  std::thread logThread_;                   // 日志处理后台线程
  std::atomic<bool> stopLogging_;           // 停止标志
  LogLevel minLogLevel_;                    // 最小日志级别
  std::size_t maxFileSize_;                 // 最大文件大小
  std::size_t currentFileSize_;             // 当前文件大小
  std::string logFormat_ =
      "{timestamp} [{level}] [{thread_id}] [{function}] {message}";  // 日志格式
};

// 辅助宏，用于简化函数名传递
#define LOG_DEBUG(message) \
  Logger::getInstance().debug([&]() { return message; }, __FUNCTION__)
#define LOG_INFO(message) \
  Logger::getInstance().info([&]() { return message; }, __FUNCTION__)
#define LOG_WARN(message) \
  Logger::getInstance().warn([&]() { return message; }, __FUNCTION__)
#define LOG_ERROR(message) \
  Logger::getInstance().error([&]() { return message; }, __FUNCTION__)

int main() {
  // 初始化日志文件，设置最大文件大小为1MB
  Logger::getInstance().initLogFile(1 * 1024 * 1024);
  // 设置日志格式
  Logger::getInstance().setLogFormat(
      "{timestamp} [{level}] [{thread_id}] [{function}] {message}");

  //
  // 记录日志示例
  LOG_DEBUG("This is a debug message");
  LOG_INFO("This is an info message");
  LOG_WARN("This is a warning message");
  LOG_ERROR("This is an error message");

  // 模拟一些操作
  for (int i = 0; i < 100; ++i) {
    LOG_INFO("Logging iteration: " + std::to_string(i));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  LOG_DEBUG("App Terminate!");

  return 0;
}

//！ 改进：1.变量和函数名改用Google c++风格命名；2.每天日志的时间可以配置选择不要日期，因为日志文件名中已经包含了日期；
