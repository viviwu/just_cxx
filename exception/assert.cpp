//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    int x, y;
    cout << "Enter two integers: ";
    cin >> x >> y;
    assert(y != 0);
    cout << "Result = " << x / y << endl;
    return 0;
}