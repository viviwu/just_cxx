/**
 ******************************************************************************
 * @file           : some_cxx11_feature.cpp.cc
 * @author         : vivi wu
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/8
 ******************************************************************************
 */

#include <iostream>
#include <memory>
#include <variant>

class MyClass : public std::enable_shared_from_this<MyClass> {
 public:
  void doSomething() {
    std::shared_ptr<MyClass> sharedPtr = shared_from_this();
    std::cout << "Doing something with sharedPtr" << std::endl;
    // 在这里可以安全地使用 sharedPtr，确保对象的有效性
  }
};

int main() {

  std::string str = "Hello";
  // Range-based for loop
  for (char c : str) {
    std::cout << c;
  }

  std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
  obj->doSomething();

  return 0;
}