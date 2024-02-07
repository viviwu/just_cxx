/**
  ******************************************************************************
  * @file           : unique_ptr.cpp.c
  * @author         : wuxw
  * @brief          : None
  ******************************************************************************
*/

#include <iostream>
#include <memory>

class MyClass {
 public:
  MyClass(int val) : value(val) {
    std::cout << "Constructing MyClass with value: " << value << std::endl;
  }

  void display() {
    std::cout << "MyClass value: " << value << std::endl;
  }

 private:
  int value;
};

int main() {
  // 使用 unique_ptr 管理动态分配的对象
  std::unique_ptr<MyClass> myClassPtr = std::make_unique<MyClass>(42);

  // 访问对象的成员函数
  myClassPtr->display();

  // unique_ptr 不能被拷贝，只能通过移动所有权
  std::unique_ptr<MyClass> anotherPtr = std::move(myClassPtr);

  // 尝试访问 myClassPtr 会导致编译错误，因为所有权已经转移
  // myClassPtr->display();

  // 使用 unique_ptr 管理动态分配的数组
  std::unique_ptr<int[]> intArrayPtr = std::make_unique<int[]>(5);
  for (int i = 0; i < 5; ++i) {
    intArrayPtr[i] = i * 2;
  }

  // 访问数组元素
  for (int i = 0; i < 5; ++i) {
    std::cout << "Element " << i << ": " << intArrayPtr[i] << std::endl;
  }

  // unique_ptr 会自动释放资源，不需要手动调用 delete
  // 不需要担心内存泄漏问题

  return 0;
}
