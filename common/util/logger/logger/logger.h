#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

class Logger {
 public:
  // Log level enumeration
  enum class LogLevel { kDebug, kInfo, kWarning, kError };

  // Singleton instance of Logger
  static Logger& Instance();

  // Delete copy constructor and assignment operator
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // Initialize log file with optional max file size and inclusion of date in
  // logs
  void InitLogFile(std::size_t max_file_size = 10 * 1024 * 1024,
                   bool include_date_in_log = true);

  // Set the minimum log level
  void SetLogLevel(LogLevel level);

  // Set the log format (supports placeholders like {timestamp}, {message},
  // etc.)
  void SetLogFormat(const std::string& format);

  // Log a message using lazy evaluation
  template <typename Func>
  void Log(LogLevel level, Func message_generator,
           const std::string& function_name);

  // Convenient methods for logging at specific levels
  template <typename Func>
  void Debug(Func message_generator, const std::string& function_name);
  template <typename Func>
  void Info(Func message_generator, const std::string& function_name);
  template <typename Func>
  void Warn(Func message_generator, const std::string& function_name);
  template <typename Func>
  void Error(Func message_generator, const std::string& function_name);

 private:
  Logger();   // Private constructor
  ~Logger();  // Private destructor

  // Create a new log file (rolling)
  void CreateNewLogFile();

  // Generate a log file name based on the current date and time
  std::string GenerateLogFileName();

  // Format the log message, including optional date
  std::string FormatLogMessage(LogLevel level, const std::string& message,
                               const std::string& function_name);

  // Replace placeholders in the log format string
  std::string ReplacePlaceholder(const std::string& format,
                                 const std::string& placeholder,
                                 const std::string& value);

  // Log processing worker thread
  void LogWorker();

  std::ofstream log_file_;  // Log file output stream
  std::mutex mutex_;        // Mutex for log file operations
  std::mutex queue_mutex_;  // Mutex for log queue
  std::condition_variable
      queue_condition_;  // Condition variable for queue processing
  std::queue<std::string> log_queue_;  // Queue to store log messages
  std::thread log_thread_;             // Background thread for logging
  std::atomic<bool> stop_logging_;     // Flag to stop logging
  LogLevel min_log_level_;             // Minimum log level
  std::size_t max_file_size_;          // Maximum log file size
  std::size_t current_file_size_;      // Current log file size
  bool include_date_in_log_;           // Flag to include date in log messages
  std::string log_format_ =
      "{timestamp} [{level}] [{thread_id}] [{function}] "
      "{message}";  // Log message format
};

// Template function to log messages lazily
template <typename Func>
void Logger::Log(LogLevel level, Func message_generator,
                 const std::string& function_name) {
  if (level < min_log_level_) return;

  // Lazy evaluation of the log message
  std::string message = message_generator();

  // Add log message to queue for asynchronous processing
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    log_queue_.emplace(FormatLogMessage(level, message, function_name));
  }
  queue_condition_.notify_one();
}

// Convenience methods for different log levels
template <typename Func>
void Logger::Debug(Func message_generator, const std::string& function_name) {
  Log(LogLevel::kDebug, message_generator, function_name);
}

template <typename Func>
void Logger::Info(Func message_generator, const std::string& function_name) {
  Log(LogLevel::kInfo, message_generator, function_name);
}

template <typename Func>
void Logger::Warn(Func message_generator, const std::string& function_name) {
  Log(LogLevel::kWarning, message_generator, function_name);
}

template <typename Func>
void Logger::Error(Func message_generator, const std::string& function_name) {
  Log(LogLevel::kError, message_generator, function_name);
}

// Macro to simplify logging with function names
#define LOG_DEBUG(message) \
  Logger::Instance().Debug([&]() { return message; }, __FUNCTION__)
#define LOG_INFO(message) \
  Logger::Instance().Info([&]() { return message; }, __FUNCTION__)
#define LOG_WARN(message) \
  Logger::Instance().Warn([&]() { return message; }, __FUNCTION__)
#define LOG_ERROR(message) \
  Logger::Instance().Error([&]() { return message; }, __FUNCTION__)

#endif  // LOGGER_H
