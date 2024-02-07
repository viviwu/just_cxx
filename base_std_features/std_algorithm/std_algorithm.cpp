//
// Created by dejavu on 2023/6/12.
//

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  std::vector<int> numbers = {4, 2, 8, 5, 1, 7};
  for (const auto &number: numbers){
    std::cout << number << " ";
  }
  std::cout <<std::endl;
  // sort
  std::sort(numbers.begin(), numbers.end());

  // find
  auto it = std::find(numbers.begin(), numbers.end(), 5);

  // max
  int maxValue = *std::max_element(numbers.begin(), numbers.end());

  // sum
  int sumValue = std::accumulate(numbers.begin(), numbers.end(), 0);

  // swap
  int a = 10, b = 20;
  std::swap(a, b);

  // replace
  std::replace(numbers.begin(), numbers.end(), 5, 10);

  // Output results
  std::cout << "Sorted Numbers: ";
  for (const auto& num : numbers) {
    std::cout << num << " ";
  }
  std::cout << "\nValue 5 found at position: " << std::distance(numbers.begin(), it) + 1 << "\n";
  std::cout << "Max Value: " << maxValue << "\n";
  std::cout << "Sum of Numbers: " << sumValue << "\n";
  std::cout << "Swapped values: a = " << a << ", b = " << b << "\n";
  std::cout << "After replacement: ";
  for (const auto& num : numbers) {
    std::cout << num << " ";
  }

  return 0;
}
