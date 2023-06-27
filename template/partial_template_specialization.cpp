//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <cstring>
using namespace std;

// ͨ��ģ��ʵ��
template <typename T>
void print_array(T arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ָ�����͵�ƫ�ػ�ʵ��
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
    // ����ͨ��ģ��ʵ��
    int arr1[] = {1, 2, 3};
    print_array(arr1, 3); // ���1 2 3

    // �������ָ�����͵�ƫ�ػ�ʵ��
    string* arr2[] = {"hello", "world", "!"};
    print_array(arr2, 3); // ���hello world !
    return 0;
}