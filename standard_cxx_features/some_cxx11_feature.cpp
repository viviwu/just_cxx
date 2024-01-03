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
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

// lambda表达式演示
void lambdaDemo() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  // 使用lambda表达式打印向量中的每个元素
  std::cout << "Lambda Demo: ";
  for (const auto& num : numbers) {
    // lambda表达式：[] (参数列表) { 函数体 }
    auto square = [](int x) { return x * x; };
    std::cout << square(num) << " ";
  }
  std::cout << std::endl;
}

// 智能指针演示
void smartPointerDemo() {
  std::cout << "Smart Pointer Demo:" << std::endl;

  // 使用std::unique_ptr创建动态分配的整数对象
  std::unique_ptr<int> ptr(new int(42));

  // 使用std::shared_ptr创建一个动态分配的字符串对象
  std::shared_ptr<std::string> strPtr = std::make_shared<std::string>("Hello");

  // 使用智能指针访问对象
  std::cout << "Value from unique_ptr: " << *ptr << std::endl;
  std::cout << "Value from shared_ptr: " << *strPtr << std::endl;

  // 不需要手动释放内存，智能指针会在合适的时候自动释放资源
}

// 并发编程演示
void concurrencyDemo() {
  std::cout << "Concurrency Demo:" << std::endl;

  // 启动一个线程
  std::thread thread([]() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Thread executed." << std::endl;
  });

  std::cout << "Main thread continues..." << std::endl;

  // 等待线程执行完毕
  thread.join();
}

int main() {
  // 自动类型推导演示
  auto number = 42;
  std::cout << "Auto Demo: " << number << std::endl;

  // 列表初始化演示
  std::vector<int> myVector = {1, 2, 3, 4, 5};
  std::cout << "List Initialization Demo: ";
  for (const auto& num : myVector) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  // 调用其他演示函数
  lambdaDemo();
  smartPointerDemo();
  concurrencyDemo();

  return 0;
}

/**
 * C++11引入了许多重要的新功能模块和特性，以下是其中一些主要的变化：

1. 自动类型推导（Type Inference）：引入了`auto`关键字，允许编译器根据初始化表达式的类型自动推导变量的类型。
2. Lambda 表达式：Lambda 表达式允许在代码中定义匿名函数，提供了一种更方便的方式来编写简短的函数对象。
3. 列表初始化（List Initialization）：允许使用统一的语法进行初始化，包括使用花括号`{}`进行初始化，可以用于初始化数组、容器和自定义类型等。
4. Range-based for 循环：引入了一种更简洁的遍历容器和数组元素的语法，通过使用`for`循环和范围声明，可以方便地遍历序列的元素。
5. 右值引用和移动语义：引入了右值引用（Rvalue Reference）和移动语义（Move Semantics），支持高效地转移资源所有权，避免不必要的内存拷贝。
6. 智能指针（Smart Pointers）：引入了`std::shared_ptr`和`std::unique_ptr`等智能指针类型，提供了更安全和方便地管理动态分配的对象的方式。
7. 并发编程支持：C++11引入了线程库（`<thread>`）和原子操作库（`<atomic>`），以及`std::mutex`和`std::condition_variable`等同步原语，使得并发编程更加容易和安全。
8. 新的标准库组件：C++11引入了许多新的标准库组件，包括元组（`std::tuple`）、正则表达式库（`<regex>`）、哈希表（`std::unordered_map`和`std::unordered_set`）等，丰富了标准库的功能。
9. 强类型枚举（Strongly Typed Enumerations）：引入了强类型枚举，可以限制枚举类型的隐式转换和作用域，提供更强的类型安全性。
10. 可变参数模板（Variadic Templates）：引入了可变参数模板，允许函数和类模板接受可变数量的参数。
11. 空指针常量和空指针检查：引入了`nullptr`关键字作为空指针常量，替代了传统的`NULL`宏，并提供了对空指针的更严格的检查。

除了上述特性之外，C++11还引入了其他一些改进，如
 静态断言（`static_assert`）、
 委托构造函数（Delegating Constructors）、
 默认和删除的函数（Defaulted and Deleted Functions）、
 类型别名（Type Aliases）等。
 这些新功能模块和特性使C++变得更加现代化、安全和高效，并提供了更好的编程体验和表达能力。
 */