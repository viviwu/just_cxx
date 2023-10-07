//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 100000; ++i) {
        counter++;
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "counter = " << counter << std::endl;

    return 0;
}

/*
 * C++11 ������ std::atomic ģ���࣬����һ���̰߳�ȫ��ԭ�ӱ������ͣ����Ա�֤�ڶ��̻߳����¶Թ�������Ĳ�����ԭ�ӵġ�
 * std::atomic ��������ʵ��һЩ�򵥵�ͬ���������������������־λ�ȣ�������ĳЩ����±������Ӹ�Ч��

 * std::atomic ģ�����ʹ�÷ǳ��򵥣�ֻ��Ҫ����һ�� std::atomic ���͵ı�����Ȼ��Ϳ�������ͨ����һ���������ж�ȡ��д�������
 */