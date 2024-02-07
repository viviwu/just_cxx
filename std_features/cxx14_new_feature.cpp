/**
 ******************************************************************************
 * @file           : cxx14_feature.cpp.c
 * @author         : vivi wu
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/8
 ******************************************************************************
 */

#include <iostream>
#include <memory>
#include <utility>
#include <tuple>
#include <iomanip>

// constexpr函数改进
constexpr int square(int n) {
    if (n < 0)
        return -1;
    else
        return n * n;
}

int main() {
    // 二进制字面量
    int binaryNum = 0b101010;
    std::cout << "Binary literal: " << binaryNum << std::endl;

    // 泛型lambda表达式
    auto lambda = [](auto x, auto y) {
        return x + y;
    };
    std::cout << "Generic lambda: " << lambda(3, 4) << std::endl;

    // 运行时数组大小
    int size;
    std::cout << "Enter the size of the array: ";
    std::cin >> size;
    int* dynamicArray = new int[size];

    // decltype(auto)
    int x = 5;
    decltype(auto) y = x;
    std::cout << "decltype(auto): " << y << std::endl;

    std::cout << "constexpr function: " << square(4) << std::endl;

    // std::make_unique
    std::unique_ptr<int> uniquePtr = std::make_unique<int>(42);
    std::cout << "std::unique_ptr: " << *uniquePtr << std::endl;

    // std::quoted
    std::string str = "Hello, World!";
    std::cout << "Quoted string: " << std::quoted(str) << std::endl;

    // std::exchange
    int value = 10;
    int oldValue = std::exchange(value, 20);
    std::cout << "Exchanged value: " << value << ", old value: " << oldValue << std::endl;

    // std::integer_sequence
    std::tuple<int, float, double> myTuple(1, 3.14f, 2.71828);
    std::cout << "Tuple elements: ";
    std::apply([](auto... args) {
        ((std::cout << args << " "), ...);
    }, myTuple);
    std::cout << std::endl;

    delete[] dynamicArray;

    return 0;
}

/**
 * C++14引入了一些新的特性和功能模块，相比于C++11，其中一些主要的改进包括：

1. 二进制字面量：C++14允许使用0b或0B前缀来表示二进制字面量，例如0b101010。

2. 泛型lambda表达式：C++14使得lambda表达式可以使用auto作为参数，从而实现泛型的lambda函数。

3. 运行时数组大小：C++14允许在编译时不确定数组的大小，可以在运行时动态指定数组的大小。

4. decltype(auto)：C++14引入了decltype(auto)类型推导，它可以根据表达式的类型自动推导变量的类型。

5. constexpr函数改进：C++14扩展了constexpr函数的能力，允许在constexpr函数中使用if语句和局部变量。

6. std::make_unique：C++14引入了std::make_unique函数，用于创建独占所有权的std::unique_ptr对象。

7. std::quoted：C++14提供了std::quoted函数，用于对字符串进行引号包裹，方便处理带有空格的字符串。

8. std::exchange：C++14引入了std::exchange函数，用于交换值并返回旧值。

9. std::integer_sequence：C++14提供了std::integer_sequence模板，用于生成整数序列，方便与模板元编程一起使用。

这些是C++14相对于C++11引入的一些主要特性和功能模块。当然，这只是其中的一部分，C++14还有其他一些细微的改进和新增功能。
 */