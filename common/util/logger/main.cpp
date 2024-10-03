#include "logger/logger.h"

int main() {
  // 初始化日志文件，设置最大文件大小为1MB
  Logger::Instance().InitLogFile(1 * 1024 * 1024);
  // 设置日志格式
  Logger::Instance().SetLogFormat(
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

  return 0;
}

/*
主要改进点总结：
	1.	日志级别过滤：setLogLevel 设置了全局日志级别，低于该级别的日志将被忽略。调用时可通过 log 函数的第一个参数指定日志级别。
	2.	延迟日志生成：日志消息只有在满足条件时才生成，通过 std::function 捕获消息生成逻辑来避免不必要的开销。
	3.	日志文件滚动：通过 initLogFile 设置最大日志文件大小，文件超过设定大小时会自动创建新的日志文件。
	4.	日志格式可配置：使用 setLogFormat 方法，用户可以自定义日志格式，支持 {timestamp}, {thread_id}, {level}, {function}, {message} 等占位符，灵活控制日志内容输出。
	5.	异步日志写入：日志通过后台线程处理，主线程只需将日志消息加入队列，避免写文件带来的性能阻塞问题。

改进后的特性：

	•	性能提升：异步写入减少了主线程的文件写入开销。
	•	多线程安全：对日志的写入操作使用了 mutex 确保线程安全，同时通过 queue 使日志生成与写入解耦。
	•	灵活性：通过自定义日志格式、日志级别过滤等功能，使 Logger 模块在不同场景下都能灵活使用。


*/
