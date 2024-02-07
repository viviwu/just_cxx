#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// 定义线程函数
void thread_func(int id)
{
    cout << "Thread " << id << " started." << endl;
    // 线程执行一些操作
    for(int i=0; i<5; i++) {
        cout << "Thread " << id << " is working." << endl;
        // 休眠1秒
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "Thread " << id << " finished." << endl;
}

int main()
{
    // 创建10个线程
    vector<thread> threads;
    for(int i=0; i<10; i++) {
        threads.emplace_back(thread_func, i);
    }

    // 等待所有线程结束
    for(auto& t : threads) {
        t.join();
    }

    cout << "All threads finished." << endl;

    return 0;
}