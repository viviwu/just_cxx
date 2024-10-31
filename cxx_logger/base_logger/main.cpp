#include <iostream>

#include "logger.h"

using namespace std;
int main() {
  // 设置日志文件
  Logger::getInstance().setLogFile("app.log");

  // 设置日志级别
  Logger::getInstance().setLogLevel(LogLevel::DEBUG);

  // 使用简化宏记录日志
  LOG_INFO("This is an info message.");
  LOG_WARNING("This is a warning message.");
  LOG_ERROR("This is an error message.");
  LOG_DEBUG("This is a debug message.");

  // cout << "Hello World!" << endl;
  return 0;
}
