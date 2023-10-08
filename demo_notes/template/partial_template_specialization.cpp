//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <cstring>
using namespace std;

// 通用模板实现
template <typename T>
void print_array(T arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// 指针类型的偏特化实现
template <typename T>
void print_array(T* arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << *arr[i] << " ";
    }
    cout << endl;
}

int main()
{
    // 测试通用模板实现
    int arr1[] = {1, 2, 3};
    print_array(arr1, 3); // 输出1 2 3

    // 测试针对指针类型的偏特化实现
    string* arr2[] = {"hello", "world", "!"};
    print_array(arr2, 3); // 输出hello world !
    return 0;
}