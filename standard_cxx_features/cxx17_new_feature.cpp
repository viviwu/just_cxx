/**
 ******************************************************************************
 * @file           : standard_cxx_feature.cpp.cc
 * @author         : vivi wu
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/8
 ******************************************************************************
 */


#include <iostream>
#include <tuple>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <functional>
#include <execution>
#include <optional>
#include <any>
#include <variant>

// 结构化绑定 // Structured binding
void structuredBindingDemo() {
    std::tuple<int, std::string, double> person{ 42, "John Doe", 3.14 };
    auto [age, name, weight] = person;
    std::cout << "Age: " << age << ", Name: " << name << ", Weight: " << weight << std::endl;
}

int performSomeOperation(){ return 3%2; }
// if语句的初始化
void ifStatementInitializationDemo() {
    if (auto result = performSomeOperation(); result > 0) {
        // 使用 result 的结果
        std::cout << "Operation succeeded. Result: " << result << std::endl;
    }
}

// 折叠表达式
template<typename... Args>
bool allTrue(Args... args) {
    return (true && ... && args);
}

// constexpr if
template<typename T>
void processValue(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integer value: " << value << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating-point value: " << value << std::endl;
    } else {
        std::cout << "Unsupported value type" << std::endl;
    }
}
// if constexpr
template<typename T>
constexpr T max(T a, T b) {
    if constexpr (std::is_const_v<T>) {
        return a;
    } else {
        return a > b ? a : b;
    }
}


// 文件系统库
void filesystemLibraryDemo() {
    std::filesystem::path filePath = "/path/to/file.txt";
    if (std::filesystem::exists(filePath)) {
        std::cout << "File exists!" << std::endl;
    } else {
        std::cout << "File does not exist." << std::endl;
    }
}



// 并行算法库
void parallelAlgorithmsDemo() {
    std::vector<int> numbers{ 1, 2, 3, 4, 5 };
#ifdef _WIN32
    // Windows-specific code
    std::cout << "This is Windows." << std::endl;
    std::for_each(std::execution::par_unseq, numbers.begin(), numbers.end(),
                  [](int& num) {
                     num *= 2;
                  });

#elif defined(__APPLE__) && defined(__MACH__)
// macOS-specific code
    std::cout << "This is macOS. clang15 not support std::execution..." << std::endl;
#else
    // Code for other operating systems
    std::cout << "This is a different operating system." << std::endl;
#endif

    std::cout << "Parallel doubling: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// 字符串视图
void stringViewDemo() {
    std::string str = "Hello, World!";
    std::string_view strView(str);
    std::cout << "String View: " << strView << std::endl;
}

//! std::optional
//! any
//! std::variant是一个可选值的容器，它用于表示可能存在或可能不存在的值。
void optionalDemo() {
    // std::optional
    std::optional<int> opt_val;
    if (opt_val.has_value()) {
        std::cout << "opt_val: " << opt_val.value() << std::endl;
    } else {
        std::cout << "opt_val ->No value." << std::endl;
    }
    std::optional<int> opt = -10;
    if (opt) {
        std::cout << *opt << '\n';
    }

    // std::any
    std::any any_val = 42;
    if (any_val.has_value()) {
        std::cout << "Value: " << std::any_cast<int>(any_val) << std::endl;
    } else {
        std::cout << "No value." << std::endl;
    }

    // std::variant
    std::variant<int, double, std::string> data = 3.14;

    if (std::holds_alternative<double>(data)) {
        std::cout << "Data: " << std::get<double>(data) << std::endl;
    } else {
        std::cout << "Data is not a double." << std::endl;
    }
}

int main_cxx17() {
    structuredBindingDemo();
    ifStatementInitializationDemo();
    bool result = allTrue(true, true, true);
    std::cout << "allTrue result: " << result << std::endl;
    processValue(42);

    // max函数if constexpr demo
    std::cout <<"max(3, 4): "<< max(3, 4) << '\n';
    std::cout <<"max(3.0, 4.0): "<< max(3.98, 4.02) << '\n';

    filesystemLibraryDemo();
    parallelAlgorithmsDemo();
    stringViewDemo();
    optionalDemo();

    return 0;
}