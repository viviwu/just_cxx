#include <iostream>
#include <fstream>
#include <string>
#include "sdk_api.h"

int main(int argc, char**argv) {
    std::ifstream file("../share/config/config.ini");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();

        printMessage();
    } else {
        std::cout << "Failed to open config file." << std::endl;
        std::cout <<argv[0]<<std::endl;
    }

    getchar();

    return 0;
}