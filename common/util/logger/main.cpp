#include "logger.hpp"

int main() {
  // 设置日志文件存放路径
//  Logger::getInstance().setLogFile("log.txt");
  Logger::getInstance().setLogLevel(LogLevel::DEBUG);

  LOG_DEBUG("This is a debug message: %d", 1);
  LOG_INFO("This is an info message: %s", "Test info");
  LOG_WARN("%s This is a warning message",__FUNCTION__ );
  LOG_ERROR("This is an error message: %f", 3.14);

  return 0;
}

