#include "logger.h"

#include <atomic>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>

// Singleton instance of Logger
Logger& Logger::Instance() {
  static Logger instance;
  return instance;
}

// Constructor
Logger::Logger() : min_log_level_(LogLevel::kDebug), stop_logging_(false) {
  log_thread_ = std::thread(&Logger::LogWorker, this);
}

// Destructor
Logger::~Logger() {
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    stop_logging_ = true;
  }
  queue_condition_.notify_all();
  if (log_thread_.joinable()) {
    log_thread_.join();
  }
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

// Initialize the log file with a maximum size and option to include the date in
// logs
void Logger::InitLogFile(std::size_t max_file_size, bool include_date_in_log) {
  std::lock_guard<std::mutex> lock(mutex_);
  max_file_size_ = max_file_size;
  include_date_in_log_ = include_date_in_log;
  current_file_size_ = 0;
  CreateNewLogFile();
}

// Set the minimum log level
void Logger::SetLogLevel(LogLevel level) { min_log_level_ = level; }

// Set the log message format
void Logger::SetLogFormat(const std::string& format) { log_format_ = format; }

// Create a new log file (rolling file)
void Logger::CreateNewLogFile() {
  if (log_file_.is_open()) {
    log_file_.close();
  }
  std::string file_name = GenerateLogFileName();
  log_file_.open(file_name, std::ios::out);
  if (!log_file_.is_open()) {
    std::cerr << "Failed to open log file: " << file_name << std::endl;
  }
  current_file_size_ = 0;
}

// Generate a log file name based on the current date and time
std::string Logger::GenerateLogFileName() {
  std::time_t now = std::time(nullptr);
  char time_str[20];
  std::strftime(time_str, sizeof(time_str), "%Y-%m-%d_%H-%M-%S",
                std::localtime(&now));
  return "log_" + std::string(time_str) + ".log";
}

// Format the log message with optional date and thread ID
std::string Logger::FormatLogMessage(LogLevel level, const std::string& message,
                                     const std::string& function_name) {
  std::string level_str;
  switch (level) {
    case LogLevel::kDebug:
      level_str = "DEBUG";
      break;
    case LogLevel::kInfo:
      level_str = "INFO";
      break;
    case LogLevel::kWarning:
      level_str = "WARNING";
      break;
    case LogLevel::kError:
      level_str = "ERROR";
      break;
  }

  // Get current time
  std::time_t now = std::time(nullptr);
  char time_str[20];
  std::strftime(time_str, sizeof(time_str),
                include_date_in_log_ ? "%Y-%m-%d %H:%M:%S" : "%H:%M:%S",
                std::localtime(&now));

  // Get current thread ID
  std::ostringstream thread_id_stream;
  thread_id_stream << std::this_thread::get_id();
  std::string thread_id_str = thread_id_stream.str();

  // Replace placeholders in log format
  std::string log_message = log_format_;
  log_message = ReplacePlaceholder(log_message, "{timestamp}", time_str);
  log_message = ReplacePlaceholder(log_message, "{thread_id}", thread_id_str);
  log_message = ReplacePlaceholder(log_message, "{level}", level_str);
  log_message = ReplacePlaceholder(log_message, "{function}", function_name);
  log_message = ReplacePlaceholder(log_message, "{message}", message);

  return log_message;
}

// Replace placeholders in the log format
std::string Logger::ReplacePlaceholder(const std::string& format,
                                       const std::string& placeholder,
                                       const std::string& value) {
  std::string result = format;
  std::size_t pos = result.find(placeholder);
  while (pos != std::string::npos) {
    result.replace(pos, placeholder.length(), value);
    pos = result.find(placeholder, pos + value.length());
  }
  return result;
}

// Log processing worker thread
void Logger::LogWorker() {
  while (!stop_logging_ || !log_queue_.empty()) {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    queue_condition_.wait(
        lock, [this]() { return stop_logging_ || !log_queue_.empty(); });

    while (!log_queue_.empty()) {
      std::string log_entry = log_queue_.front();
      log_queue_.pop();
      lock.unlock();

      std::lock_guard<std::mutex> file_lock(mutex_);
      if (log_file_.is_open()) {
        log_file_ << log_entry << std::endl;
        current_file_size_ += log_entry.size();

        // Check if we need to roll the log file
        if (current_file_size_ >= max_file_size_) {
          CreateNewLogFile();
        }
      }

      lock.lock();
    }
  }
}
