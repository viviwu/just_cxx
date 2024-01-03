/**
 ******************************************************************************
 * @file           : multi_threads.cpp.c
 * @author         : viviwu
 * @brief          : None
 * @attention      : None
 * @date           : 1/3/24
 ******************************************************************************
 */

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;            // 创建一个互斥锁
std::condition_variable cv;  // 创建一个条件变量

int tickets         = 100;  // 假设有100张票
int soldTickets     = 0;    // 已售出的票数
int returnedTickets = 0;    // 已退回的票数

void sellTickets(int windowId) {
  while (true) {
    std::unique_lock<std::mutex> lock(mutex);  // 使用 unique_lock 操作互斥锁

    // 判断是否还有票可售
    if (tickets > 0) {
      std::cout << "窗口 " << windowId << " 售出票号 " << tickets << std::endl;
      tickets--;
      soldTickets++;
    } else {
      break;
    }

    lock.unlock();  // 解锁

    // 唤醒其他线程，以便有人退票
    cv.notify_all();
  }
}

void returnTickets() {
  while (true) {
    std::unique_lock<std::mutex> lock(mutex);  // 使用 unique_lock 操作互斥锁

    // 检查是否需要停止退票
    if (soldTickets >= 50 || returnedTickets >= 30) {
      break;
    }

    // 等待条件满足，即可以退票
    cv.wait(lock, [] { return soldTickets >= 50 || returnedTickets >= 30; });

    // 判断是否还可以退票
    if (returnedTickets < 30) {
      std::cout << "退回票号 " << tickets + 1 << std::endl;
      tickets++;
      returnedTickets++;
    } else {
      break;
    }

    lock.unlock();  // 解锁

    // 唤醒其他线程，以便有人购买
    cv.notify_all();
  }
}

int main() {
  const int numWindows = 4;  // 假设有4个窗口
  std::thread windows[numWindows];
  std::thread returnThread(returnTickets);

  // 创建多个窗口线程
  for (int i = 0; i < numWindows; ++i) {
    windows[i] = std::thread(sellTickets, i + 1);
  }

  // 等待所有窗口线程结束
  for (int i = 0; i < numWindows; ++i) {
    windows[i].join();
  }

  returnThread.join();

  return 0;
}