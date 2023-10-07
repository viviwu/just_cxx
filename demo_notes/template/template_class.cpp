//
// Created by dejavu on 2023/6/12.
//
#include <iostream>

using namespace std;

// 定义一个类模板
template<typename T>
class Stack {
private:
    T* data;
    int capacity;
    int top;

public:
    Stack(int size) {
        data = new T[size];
        capacity = size;
        top = -1;
    }

    ~Stack() {
        delete[] data;
    }

    void push(T value) {
        if (top < capacity - 1) {
            data[++top] = value;
        }
    }

    T pop() {
        if (top >= 0) {
            return data[top--];
        }
        return T();
    }
};

int main() {
    // 实例化类模板
    Stack<int> intStack(10);
    intStack.push(5);
    intStack.push(10);
    cout << "Popped " << intStack.pop() << endl;
    cout << "Popped " << intStack.pop() << endl;

    Stack<double> doubleStack(10);
    doubleStack.push(3.14);
    doubleStack.push(2.71);
    cout << "Popped " << doubleStack.pop() << endl;
    cout << "Popped " << doubleStack.pop() << endl;

    return 0;
}