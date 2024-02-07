//
// Created by dejavu on 2023/6/13.
//

#include <iostream>
#include <cmath>
#include <complex>

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


// 通用模板实现
template <typename T>
T square_root(T x)
{
    return sqrt(x);
}

// 复数类型的模板特化实现
template <>
complex<double> square_root(complex<double> x)


{
    double real = x.real();
    double imag = x.imag();
    double r = sqrt(real * real + imag * imag);
    double theta = atan2(imag, real);
    return complex<double>(sqrt(r) * cos(theta / 2), sqrt(r) * sin(theta / 2));
}


int main()
{
  // 测试通用模板实现
  int arr1[] = {1, 2, 3};
  print_array(arr1, 3); // 输出1 2 3

  // 测试针对指针类型的偏特化实现
  string* arr2[] = {"hello", "world", "!"};
  print_array(arr2, 3); // 输出hello world !

  /*****************************************/

    // 测试通用模板实现
    cout << square_root(4) << endl; // 输出2
    cout << square_root(2.25) << endl; // 输出1.5

    // 测试针对复数类型的模板特化实现
    complex<double> c(3, 4);
    cout << square_root(c) << endl; // 输出(2, 1)
    return 0;
}
/*
在上面的代码中，我们定义了一个求平方根的函数模板square_root。
 在通用模板实现中，我们使用C++标准库中的sqrt函数来计算平方根。
 在针对复数类型的模板特化实现中，我们使用极坐标法来计算复数的平方根。
在主函数中，我们对通用模板实现和针对复数类型的模板特化实现进行了测试，并输出了计算结果。
*/