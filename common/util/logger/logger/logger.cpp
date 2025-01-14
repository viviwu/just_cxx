#include "logger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <mutex>
#include <cstdarg>

// Logger constructor
Logger::Logger()
    : stop_thread_(false),
      current_level_(kDebug),
      max_file_size_(kDefaultMaxFileSize),
      log_directory_("./") {
  // Start the log processing thread
  log_thread_ = std::thread(&Logger::LogProcessingThread, this);
}

// Logger destructor
Logger::~Logger() {
  stop_thread_ = true;
  cv_.notify_all();  // Notify the log processing thread to stop.

  if (log_thread_.joinable()) {
    log_thread_.join();  // Ensure the log processing thread finishes
  }

  // Close the log file safely
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

// Get singleton instance of Logger
Logger& Logger::GetInstance() {
  static Logger instance;
  return instance;
}

// Set the log level
void Logger::SetLogLevel(LogLevel level) {
  current_level_ = level;
}

// Set the directory where log files will be stored
void Logger::SetLogDirectory(const std::string& directory) {
  std::lock_guard<std::mutex> lock(mutex_);
  log_directory_ = directory;
}

// Log a message with the specified log level
void Logger::Log(LogLevel level, const char* format, ...) {
  if (!ShouldLog(level)) return;

  // Use unique_lock for thread safety
  std::unique_lock<std::mutex> lock(mutex_);

  // Get current timestamp
  std::string current_time = GetCurrentTime();

  // Determine log level string
  std::string log_level_str;
  switch (level) {
    case kDebug: log_level_str = "DEBUG"; break;
    case kInfo: log_level_str = "INFO"; break;
    case kWarn: log_level_str = "WARN"; break;
    case kError: log_level_str = "ERROR"; break;
  }

  // Format the log message
  va_list args;
  va_start(args, format);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  // Convert thread ID to string
  std::stringstream thread_id_stream;
  thread_id_stream << std::this_thread::get_id();

  // Combine everything into the final log message
  std::string message = "[" + current_time + "][" + thread_id_stream.str() + "][" + log_level_str + "] " + buffer;

  // Put the log message into the queue
  log_queue_.push(message);
  cv_.notify_one();  // Notify the log processing thread to process the log
}

// The log processing thread function
void Logger::LogProcessingThread() {
  while (!stop_thread_) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !log_queue_.empty() || stop_thread_; });

    while (!log_queue_.empty()) {
      std::string message = log_queue_.front();
      log_queue_.pop();

      // Write to both file and console
      WriteToFile(message);
      WriteToConsole(message);
    }
  }
}

// Get the current timestamp as a string
std::string Logger::GetCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

// Check if the log level is enabled for logging
bool Logger::ShouldLog(LogLevel level) {
  return level >= current_level_;
}

// Write log message to the file
void Logger::WriteToFile(const std::string& message) {
  CheckAndCreateLogFile();

  if (log_file_.is_open()) {
    log_file_ << message << std::endl;
  } else {
    std::cerr << "Failed to write to log file!" << std::endl;
  }
}

// Write log message to the console
void Logger::WriteToConsole(const std::string& message) {
  std::cout << message << std::endl;
}

// Check if the log file exists and is writable, and create a new one if necessary
void Logger::CheckAndCreateLogFile() {
  // Open log file if not already opened
  if (!log_file_.is_open()) {
    std::string log_filename = GetLogFileName();
    log_file_.open(log_filename, std::ios_base::app);  // Open in append mode

    if (!log_file_.is_open()) {
      std::cerr << "Failed to open log file: " << log_filename << std::endl;
    }
  }

  // Check if the file size exceeds max size and roll over if necessary
  if (log_file_.tellp() >= max_file_size_) {
    log_file_.close();  // Close the current file
    log_file_.open(GetLogFileName(), std::ios_base::app);  // Open a new file
  }
}

// Generate the log file name based on the current date
std::string Logger::GetLogFileName() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm;
  localtime_s(&tm, &time);

  std::stringstream ss;
  ss << log_directory_ << "/log_" << std::put_time(&tm, "%Y-%m-%d") << ".log";
  return ss.str();
}
