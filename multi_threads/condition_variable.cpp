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
���ʹ����������ʵ�ֶ�������ߺ������ߣ�
��α��������ͼ������⣿
���ʹ����������ʵ��������-������ģ�͵��н���У�

using namespace std;

// ������Դ
int counter = 0;

// ����������������
//mutex mtx;
//condition_variable cv;

// ��һ���̵߳ĺ���
void threadFunction1() {
    for (int i = 0; i < 1000000; ++i) {
        // ����
        unique_lock<mutex> lock(mtx);

        // �޸Ĺ�����Դ
        counter++;

        // ֪ͨ�ȴ������������ϵ��߳�
        cv.notify_one();
    }
}

// �ڶ����̵߳ĺ���
void threadFunction2() {
    // ����
    unique_lock<mutex> lock(mtx);

    // �ȴ���������
    cv.wait(lock, []{ return counter >= 1000000; });

    // �޸Ĺ�����Դ
    counter--;

    // ���������Դ��ֵ
    cout << "Counter = " << counter << endl;
}

int main2() {
    // ���������߳�
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    // �ȴ������߳����
    t1.join();
    t2.join();

    return 0;
}

//�ڵ�һ���̵߳ĺ����У�����ʹ�û������Թ�����Դ�����޸ģ���ʹ�� notify_one ����֪ͨ�ȴ������������ϵ��̡߳�
// �ڵڶ����̵߳ĺ����У�����ʹ�û������� wait �����ȴ�����������������������ʱ�Թ�����Դ�����޸ģ��������ֵ��
//
//ͨ��ʹ���������������ǿ��Ը���Ч�صȴ�������Դ�����ض�������������æ�ȴ���


//һ���߳̿���ͨ���������������� wait() �������ȴ���һ���̷߳����źţ����ڽ��յ��źź����ִ�С�
// ��һ���߳̿���ͨ���������������� notify_one() �� notify_all() �����������źŸ��ȴ����̡߳�
// ���磺

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