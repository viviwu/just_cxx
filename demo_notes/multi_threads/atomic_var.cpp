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
 * C++11 引入了 std::atomic 模板类，它是一种线程安全的原子变量类型，可以保证在多线程环境下对共享变量的操作是原子的。
 * std::atomic 可以用于实现一些简单的同步操作，比如计数器、标志位等，并且在某些情况下比锁更加高效。

 * std::atomic 模板类的使用非常简单，只需要定义一个 std::atomic 类型的变量，然后就可以像普通变量一样对它进行读取和写入操作。
 */