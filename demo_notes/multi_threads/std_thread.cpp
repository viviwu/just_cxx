#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// �����̺߳���
void thread_func(int id)
{
    cout << "Thread " << id << " started." << endl;
    // �߳�ִ��һЩ����
    for(int i=0; i<5; i++) {
        cout << "Thread " << id << " is working." << endl;
        // ����1��
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "Thread " << id << " finished." << endl;
}

int main()
{
    // ����10���߳�
    vector<thread> threads;
    for(int i=0; i<10; i++) {
        threads.emplace_back(thread_func, i);
    }

    // �ȴ������߳̽���
    for(auto& t : threads) {
        t.join();
    }

    cout << "All threads finished." << endl;

    return 0;
}