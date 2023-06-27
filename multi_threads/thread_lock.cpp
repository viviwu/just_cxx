//
// Created by dejavu on 2023/6/11.
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// 共享资源
int counter = 0;

// 互斥锁
mutex mtx;

// 第一个线程的函数
void threadFunction1() {
    for (int i = 0; i < 1000000; ++i) {
        // 加锁
        mtx.lock();
        counter++;
        // 解锁
        mtx.unlock();
    }
}

// 第二个线程的函数
void threadFunction2() {
    for (int i = 0; i < 1000000; ++i) {
        // 加锁
        mtx.lock();
        counter--;
        // 解锁
        mtx.unlock();
    }
}

int main() {
    // 创建两个线程
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    // 等待两个线程完成
    t1.join();
    t2.join();

    // 输出共享资源的值
    cout << "Counter = " << counter << endl;

    return 0;
}