//
// Created by viviwu on 2023/8/14.
//

#include <iostream>

enum Color: unsigned int {
    Red = 1 << 0,
    Green = 1 << 1,
    Blue = 1 << 2
};

int check_color() {
    //检查多个值的组合:
    Color col = static_cast<Color>(Color::Red | Color::Green);
    if (col & Color::Green) {
        // c contains Green
        std::cout << "c contains Green "<< std::endl;
    }
    std::cout << std::hex << col << std::endl; // 3

    Color c1 = Color::Red;
    Color c2 = Color::Green;

    std::cout << std::hex << (c1 ^ c2) << std::endl; // 3

    return 0;
}
