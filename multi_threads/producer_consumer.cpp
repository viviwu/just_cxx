//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

const int kBufferSize = 10;

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer(int id)
{
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (q.size() == kBufferSize)
        {
            cv.wait(lock);
        }
        q.push(i);
        std::cout << "Producer " << id << " produced " << i << std::endl;
        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer(int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (q.empty())
        {
            cv.wait(lock);
        }
        int x = q.front();
        q.pop();
        std::cout << "Consumer " << id << " consumed " << x << std::endl;
        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::vector<std::thread> producers;
    for (int i = 0; i < 2; ++i)
    {
        producers.emplace_back(producer, i);
    }
    std::vector<std::thread> consumers;
    for (int i = 0; i < 2; ++i)
    {
        consumers.emplace_back(consumer, i);
    }
    for (auto& t : producers)
    {
        t.join();
    }
    for (auto& t : consumers)
    {
        t.join();
    }
    return 0;
}

/*
 *
 在这个例子中，有两个生产者和两个消费者，它们共享一个队列作为缓冲区。在生产者线程中，先获取互斥锁，然后判断队列是否已满，如果已满就等待条件变量。如果条件变量通知到来，就将数据压入队列，并输出生产的数据。然后释放锁并通知所有等待的线程。在消费者线程中，先获取互斥锁，然后判断队列是否为空，如果为空就等待条件变量。如果条件变量通知到来，就从队列中取出数据，并输出消费的数据。然后释放锁并通知所有等待的线程。

需要注意的是，在等待条件变量时，应该使用 while 循环来检查条件是否满足，而不是使用 if 语句。这是因为条件变量可能出现虚假唤醒的情况，即线程在没有收到通知的情况下被唤醒，如果使用 if 语句，就可能会导致线程误以为条件已经满足，从而出现错误。

另外，为了避免死锁问题，需要保证多个线程以相同的顺序获取锁。在本例中，生产者和消费者线程使用的是相同的互斥锁，因此它们会以相同的顺序获取锁。同时，在等待条件变量时，需要使用 std::unique_lockstd::mutex 类型的锁，而不能使用 std::lock_guardstd::mutex 类型的锁，以便在等待中间释放锁，从而避免不必要的等待。
 * */