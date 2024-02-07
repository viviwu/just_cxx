//
// Created by dejavu on 2023/6/12.
//


#include <iostream>

template <typename T>
T myMax(T a, T b) {
  return a > b ? a : b;
}

int main() {

  std::cout << "Max of 5 and 10: " << myMax<int>(5, 10) << std::endl;
  std::cout << "Max of 3.14 and 2.71: " << myMax<double>(3.14, 2.71) << std::endl;
  std::cout << "Max of 'A' and 'Z': " << myMax<char>('A', 'Z') << std::endl;

  return 0;
}
