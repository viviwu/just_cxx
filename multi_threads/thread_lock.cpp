//
// Created by dejavu on 2023/6/11.
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// ������Դ
int counter = 0;

// ������
mutex mtx;

// ��һ���̵߳ĺ���
void threadFunction1() {
    for (int i = 0; i < 1000000; ++i) {
        // ����
        mtx.lock();
        counter++;
        // ����
        mtx.unlock();
    }
}

// �ڶ����̵߳ĺ���
void threadFunction2() {
    for (int i = 0; i < 1000000; ++i) {
        // ����
        mtx.lock();
        counter--;
        // ����
        mtx.unlock();
    }
}

int main() {
    // ���������߳�
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    // �ȴ������߳����
    t1.join();
    t2.join();

    // ���������Դ��ֵ
    cout << "Counter = " << counter << endl;

    return 0;
}