//
// Created by dejavu on 2023/6/12.
//
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer()
{
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        q.push(i);
        std::cout << "Producer produced " << i << std::endl;
        lock.unlock();
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []() { return !q.empty(); });
        int x = q.front();
        q.pop();
        std::cout << "Consumer consumed " << x << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

/*
如何使用条件变量实现多个生产者和消费者？
如何避免死锁和饥饿问题？
如何使用条件变量实现生产者-消费者模型的有界队列？

using namespace std;

// 共享资源
int counter = 0;

// 互斥锁和条件变量
//mutex mtx;
//condition_variable cv;

// 第一个线程的函数
void threadFunction1() {
    for (int i = 0; i < 1000000; ++i) {
        // 加锁
        unique_lock<mutex> lock(mtx);

        // 修改共享资源
        counter++;

        // 通知等待在条件变量上的线程
        cv.notify_one();
    }
}

// 第二个线程的函数
void threadFunction2() {
    // 加锁
    unique_lock<mutex> lock(mtx);

    // 等待条件变量
    cv.wait(lock, []{ return counter >= 1000000; });

    // 修改共享资源
    counter--;

    // 输出共享资源的值
    cout << "Counter = " << counter << endl;
}

int main2() {
    // 创建两个线程
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    // 等待两个线程完成
    t1.join();
    t2.join();

    return 0;
}

//在第一个线程的函数中，我们使用互斥锁对共享资源进行修改，并使用 notify_one 函数通知等待在条件变量上的线程。
// 在第二个线程的函数中，我们使用互斥锁和 wait 函数等待条件变量，并在条件满足时对共享资源进行修改，并输出其值。
//
//通过使用条件变量，我们可以更有效地等待共享资源满足特定条件，而不是忙等待。


//一个线程可以通过调用条件变量的 wait() 函数来等待另一个线程发送信号，并在接收到信号后继续执行。
// 另一个线程可以通过调用条件变量的 notify_one() 或 notify_all() 函数来发送信号给等待的线程。
// 例如：

bool ready = false;

void print(int id)
{
    std::unique_lock<std::mutex> lock(mtx);
    while (!ready)
    {
        cv.wait(lock);
    }
    std::cout << "Thread " << id << " is running" << std::endl;
}

int main3()
{
    std::thread t1(print, 1);
    std::thread t2(print, 2);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ready = true;
    cv.notify_all();
    t1.join();
    t2.join();
    return 0;
}
 */