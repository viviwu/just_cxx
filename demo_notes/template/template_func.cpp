//
// Created by dejavu on 2023/6/12.
//


#include <iostream>

using namespace std;

// ����һ������ģ��
template <typename T>
T max(T x, T y) {
    return (x > y) ? x : y;
}

int main() {
    // ʵ��������ģ��
    int a = 5, b = 10;
    cout << "Max of " << a << " and " << b << " is " << max(a, b) << endl;

    double c = 3.14, d = 2.71;
    cout << "Max of " << c << " and " << d << " is " << max(c, d) << endl;

    return 0;
}
