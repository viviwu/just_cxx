/**
 * @file   logger.hpp
 * @brief  class description
 * @author wuxw
 * @date   2025/1/14/014
 * @version 1.0
 *
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <thread>
#include <atomic>

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR
};

class Logger {
 public:
  static Logger& getInstance() {
    static Logger instance;
    return instance;
  }

  void setLogLevel(LogLevel level) {
    logLevel_.store(level);
  }

  template<typename... Args>
  void log(LogLevel level, const std::string& format, Args... args) {
    if (level < logLevel_.load()) {
      return;
    }

    std::lock_guard<std::mutex> lock(dateCheckMutex_);
    checkRollover();

    std::stringstream ss;
    formatLog(ss, level);
    formatMessage(ss, format, args...);

    std::lock_guard<std::mutex> logLock(logMutex_);
    std::cout << ss.str();
    if (logFile_.is_open()) {
      logFile_ << ss.str();
      logFile_.flush();
    }
  }

 private:
  Logger() {
    auto start_time = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start_time).count();
    runIdentifier_ = std::to_string(start_ms);

    setLogFile();
  }

  ~Logger() {
    if (logFile_.is_open()) {
      logFile_.close();
    }
  }

  void setLogFile() {
    std::lock_guard<std::mutex> lock(fileMutex_);
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    std::stringstream filename;
    filename << "log_" << "_" << std::put_time(&now_tm, "%Y-%m-%d") << runIdentifier_ << ".log";

    if (logFile_.is_open()) {
      logFile_.close();
    }
    logFile_.open(filename.str(), std::ios::out | std::ios::app);
    currentDay_ = now_tm.tm_mday;
  }

  void checkRollover() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    if (now_tm.tm_mday != currentDay_) {
      if (logFile_.is_open()) {
        logFile_.close();
      }

      std::stringstream oldFilename;
      oldFilename << "log_" << runIdentifier_ << "_" << std::put_time(&lastTime_, "%Y-%m-%d") << ".log";

      std::stringstream newFilename;
      newFilename << "log_" << runIdentifier_ << "_" << std::put_time(&lastTime_, "%Y-%m-%d") << "_archived.log";

      // Rename the old log file
      if (std::rename(oldFilename.str().c_str(), newFilename.str().c_str()) != 0) {
        std::cerr << "Error renaming log file: " << oldFilename.str() << " to " << newFilename.str() << std::endl;
      }

      setLogFile();
      lastTime_ = now_tm;
    }
  }

  void formatLog(std::stringstream& ss, LogLevel level) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    ss << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "]"
       << "[" << std::this_thread::get_id() << "]"
       << "[" << logLevelToString(level) << "] ";
  }

  std::string logLevelToString(LogLevel level) {
    switch (level) {
      case LogLevel::DEBUG: return "DEBUG";
      case LogLevel::INFO:  return "INFO";
      case LogLevel::WARN:  return "WARN";
      case LogLevel::ERROR: return "ERROR";
      default:              return "UNKNOWN";
    }
  }

  template<typename... Args>
  void formatMessage(std::stringstream& ss, const std::string& format, Args... args) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), format.c_str(), args...);
    ss << buffer << std::endl;
  }

  std::ofstream logFile_;
  std::mutex logMutex_;
  std::mutex fileMutex_;
  std::mutex dateCheckMutex_;
  std::atomic<LogLevel> logLevel_;
  std::string runIdentifier_;
  int currentDay_;
  std::tm lastTime_;
};

#define LOG_DEBUG(format, ...) Logger::getInstance().log(LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  Logger::getInstance().log(LogLevel::INFO,  format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  Logger::getInstance().log(LogLevel::WARN,  format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::getInstance().log(LogLevel::ERROR, format, ##__VA_ARGS__)

#endif // LOGGER_H