//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <thread>
#include <shared_mutex>

std::shared_mutex rw_mutex;  // 读写锁

int data = 0;  // 共享数据

void writer() {
    for (int i = 0; i < 5; i++) {
        // 获取写锁
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        // 写入共享数据
        data++;

        // 输出写入的值
        std::cout << "writer thread: " << std::this_thread::get_id() << " write data: " << data << std::endl;

        // 释放写锁
        lock.unlock();

        // 等待一段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void reader() {
    for (int i = 0; i < 5; i++) {
        // 获取读锁
        std::shared_lock<std::shared_mutex> lock(rw_mutex);

        // 读取共享数据
        int value = data;

        // 输出读取的值
        std::cout << "reader thread: " << std::this_thread::get_id() << " read data: " << value << std::endl;

        // 释放读锁
        lock.unlock();

        // 等待一段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // 创建 3 个读线程和 1 个写线程
    std::thread t1(reader);
    std::thread t2(writer);
    std::thread t3(reader);
    std::thread t4(reader);

    // 等待线程结束
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
