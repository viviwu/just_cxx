/**
 ******************************************************************************
 * @file           : cxx14_feature.cpp.c
 * @author         : vivi wu
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/8
 ******************************************************************************
 */
//
#include <iostream>
#include <string>

// 字符串字面量 后缀操作符（string literal suffix operator）
auto operator "" _str(const char* str, std::size_t len) {
    return std::string(str, len);
}

// 整数字面量后缀操作符
constexpr long long operator "" _ms(unsigned long long milliseconds) {
    return milliseconds;
}

// 浮点数字面量后缀操作符
constexpr double operator "" _km(long double kilometers) {
    return kilometers;
}

int main_cxx14(){

    // 字面量字符串
    std::cout << "Hello"_str << '\n';

    auto time = 5000_ms;  // 5000毫秒
    std::cout << "Time: " << time << " milliseconds" << std::endl;

    auto distance = 10.5_km;  // 10.5公里
    std::cout << "Distance: " << distance << " kilometers" << std::endl;

    return 0;
}