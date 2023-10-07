//
// Created by dejavu on 2023/6/12.
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// �ݹ���
recursive_mutex mtx;

void recursiveFunction(int depth) {
    // ����
    mtx.lock();

    cout << "Depth " << depth << ": locked" << endl;

    if (depth > 1) {
        recursiveFunction(depth - 1);
    }

    cout << "Depth " << depth << ": unlocked" << endl;

    // ����
    mtx.unlock();
}

int main() {
    // ���õݹ麯��
    recursiveFunction(5);

    return 0;
}

/*
���� std::recursive_mutex ���ǵݹ�ģ������ͬһ�߳��ж��ʹ�� lock ����Ҳ�ǰ�ȫ�ġ�
*/