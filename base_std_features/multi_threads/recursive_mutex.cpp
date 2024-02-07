//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// 递归锁
recursive_mutex mtx;

void recursiveFunction(int depth) {
    // 加锁
    mtx.lock();

    cout << "Depth " << depth << ": locked" << endl;

    if (depth > 1) {
        recursiveFunction(depth - 1);
    }

    cout << "Depth " << depth << ": unlocked" << endl;

    // 解锁
    mtx.unlock();
}

int main() {
    // 调用递归函数
    recursiveFunction(5);

    return 0;
}

/*
由于 std::recursive_mutex 类是递归的，因此在同一线程中多次使用 lock 函数也是安全的。
*/