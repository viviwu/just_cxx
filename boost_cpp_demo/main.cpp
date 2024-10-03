#include <iostream>
#include <type_traits>

using namespace std;

// 定义一个函数模板，根据传入的类型是否是整型来打印不同的信息
template <typename T>
void printTypeProperties() {
  std::cout << "Type: " << typeid(T).name() << std::endl;
  std::cout << "Is integral: " << std::is_integral<T>::value << std::endl;
  std::cout << "Is floating point: " << std::is_floating_point<T>::value
            << std::endl;
  std::cout << "Is const: " << std::is_const<T>::value << std::endl;
  std::cout << "Is volatile: " << std::is_volatile<T>::value << std::endl;
  std::cout << std::endl;
}

int main() {
  printTypeProperties<int>();              // 测试整型
  printTypeProperties<double>();           // 测试浮点型
  printTypeProperties<const int*>();       // 测试 const 指针
  printTypeProperties<volatile double>();  // 测试 volatile 类型

  return 0;
}
