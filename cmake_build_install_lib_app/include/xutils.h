//
// Created by dejavu on 2023/11/16.
//

#ifndef SDKAPI_XUTILS_H
#define SDKAPI_XUTILS_H


#include <string>
#include <vector>

std::vector<std::string> split_string(std::string source_str, std::string pattern);

std::string extractVersion(const std::string& instr);

int compareVersion(const std::string& version1, const std::string& version2);

#endif //SDKAPI_XUTILS_H
