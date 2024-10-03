#include <iostream>
#include "sts_user_def.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Holder huser={0, "xxx corporation", "", "", 0, 0};
    std::cout << huser.name << std::endl;

    return 0;
}
