//
// Created by viviwu on 2022/4/8.
//

#include <iostream>

#include <thread>

std::thread::id main_thread_id = std::this_thread::get_id();

void hello()
{
  std::cout << "Hello Concurrent World\n";
  if (main_thread_id == std::this_thread::get_id())
    std::cout << "This is the main thread.\n";
  else
    std::cout << "This is not the main thread.\n";
}

void pause_thread(int n) {
  std::this_thread::sleep_for(std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}

int std_thread()
{
  std::thread thr(hello);
  std::cout <<"hardware_concurrency: "<< thr.hardware_concurrency() << std::endl;   //可以并发执行多少个(不准确)
  std::cout << "native_handle: " << thr.native_handle() << std::endl;   //可以并发执行多少个(不准确)
  thr.join();
  
  std::thread thr2(hello);
  thr2.detach();

  std::thread threads[5];                         // 默认构造线程
  std::cout << "Spawning 5 threads...\n";
  for (int i = 0; i < 5; ++i)
    threads[i] = std::thread(pause_thread, i + 1);   // move-assign threads
  std::cout << "Done spawning threads. Now waiting for them to join:\n";
  for (auto &thread : threads)
    thread.join();
  std::cout << "All threads joined!\n";
  return 0;
}