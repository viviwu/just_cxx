//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <thread>
#include <shared_mutex>

std::shared_mutex rw_mutex;  // ��д��

int data = 0;  // ��������

void writer() {
    for (int i = 0; i < 5; i++) {
        // ��ȡд��
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        // д�빲������
        data++;

        // ���д���ֵ
        std::cout << "writer thread: " << std::this_thread::get_id() << " write data: " << data << std::endl;

        // �ͷ�д��
        lock.unlock();

        // �ȴ�һ��ʱ��
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void reader() {
    for (int i = 0; i < 5; i++) {
        // ��ȡ����
        std::shared_lock<std::shared_mutex> lock(rw_mutex);

        // ��ȡ��������
        int value = data;

        // �����ȡ��ֵ
        std::cout << "reader thread: " << std::this_thread::get_id() << " read data: " << value << std::endl;

        // �ͷŶ���
        lock.unlock();

        // �ȴ�һ��ʱ��
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // ���� 3 �����̺߳� 1 ��д�߳�
    std::thread t1(reader);
    std::thread t2(writer);
    std::thread t3(reader);
    std::thread t4(reader);

    // �ȴ��߳̽���
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
