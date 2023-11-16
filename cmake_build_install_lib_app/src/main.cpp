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

        std::string ver1=extractVersion(line);
        std::cout << ver1 << std::endl;
        std::string ver2=extractVersion("pai_3.14.15_926_.");
        std::cout << ver2 << std::endl;
        int ret = compareVersion(ver1, ver2);
        if(ret>0){
            std::cout << ver1<<">" <<ver2<< std::endl;
        }else{
            std::cout << ver1<<"<" <<ver2<< std::endl;
        }

        std::string teststr="E:\\DevSpace\\gitsync\\just_cxx\\cmake_build_install_lib_app\\cmake-build-debug";
        std::vector<std::string> sp = split_string(teststr, "\\");
        for (int i = 0; i < sp.size(); ++i) {
            std::cout << i<<sp[i]<< std::endl;
        }

    } else {
        std::cout << "Failed to open config file." << std::endl;
        std::cout <<argv[0]<<std::endl;
    }

//    getchar();

    return 0;
}

