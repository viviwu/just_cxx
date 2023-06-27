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
 ����������У������������ߺ����������ߣ����ǹ���һ��������Ϊ�����������������߳��У��Ȼ�ȡ��������Ȼ���ж϶����Ƿ���������������͵ȴ����������������������֪ͨ�������ͽ�����ѹ����У���������������ݡ�Ȼ���ͷ�����֪ͨ���еȴ����̡߳����������߳��У��Ȼ�ȡ��������Ȼ���ж϶����Ƿ�Ϊ�գ����Ϊ�վ͵ȴ����������������������֪ͨ�������ʹӶ�����ȡ�����ݣ���������ѵ����ݡ�Ȼ���ͷ�����֪ͨ���еȴ����̡߳�

��Ҫע����ǣ��ڵȴ���������ʱ��Ӧ��ʹ�� while ѭ������������Ƿ����㣬������ʹ�� if ��䡣������Ϊ�����������ܳ�����ٻ��ѵ���������߳���û���յ�֪ͨ������±����ѣ����ʹ�� if ��䣬�Ϳ��ܻᵼ���߳�����Ϊ�����Ѿ����㣬�Ӷ����ִ���

���⣬Ϊ�˱����������⣬��Ҫ��֤����߳�����ͬ��˳���ȡ�����ڱ����У������ߺ��������߳�ʹ�õ�����ͬ�Ļ�������������ǻ�����ͬ��˳���ȡ����ͬʱ���ڵȴ���������ʱ����Ҫʹ�� std::unique_lockstd::mutex ���͵�����������ʹ�� std::lock_guardstd::mutex ���͵������Ա��ڵȴ��м��ͷ������Ӷ����ⲻ��Ҫ�ĵȴ���
 * */