//
// Created by dejavu on 2023/6/12.
//

#include <algorithm>


#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

int main() {
    // 初始化一个整数向量
    std::vector<int> nums = {5, 3, 1, 4, 2};

    // 使用sort函数进行排序
    std::sort(nums.begin(), nums.end());
    std::cout << "排序后的向量：";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 使用find函数查找元素
    int target = 3;
    auto it = std::find(nums.begin(), nums.end(), target);
    if (it != nums.end()) {
        std::cout << "找到元素 " << target << " 在位置 " << (it - nums.begin()) << std::endl;
    } else {
        std::cout << "未找到元素 " << target << std::endl;
    }

    // 使用max_element函数找到最大值
    auto max_it = std::max_element(nums.begin(), nums.end());
    std::cout << "最大值为：" << *max_it << std::endl;

    // 使用accumulate函数计算向量的和
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "向量的和为：" << sum << std::endl;

    // 使用swap函数交换两个元素
    std::swap(*nums.begin(), *(nums.begin() + 1));
    std::cout << "交换后的向量：";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 使用replace函数替换所有等于目标值的元素
    int old_val = 1;
    int new_val = 9;
    std::replace(nums.begin(), nums.end(), old_val, new_val);
    std::cout << "替换后的向量：";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 数字信号处理示例：计算向量的平均值
    double avg = static_cast<double>(sum) / nums.size();
    std::cout << "向量的平均值为：" << avg << std::endl;

    return 0;
}

/**
c++ algorithm library
c++ algorithm sort
c++ algorithm find
c++ algorithm max
c++ algorithm sum
c++ algorithm min
c++ algorithm swap
c++ algorithm replace
c++ algorithms for digital signal processing
*/
