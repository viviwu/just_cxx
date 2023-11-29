//
// Created by dejavu on 2023/11/16.
//

#include "xutils.h"

#include <iostream>
#include <regex>
#include <string>


std::vector<std::string> split_string(std::string source_str, std::string pattern){
    std::vector<std::string> result;
    char *str = const_cast<char*>(source_str.c_str());
    const char *delim = pattern.c_str();
    char *p;
    p = strtok(str, delim); // strtok_r
    while(p)
    {
//        printf("%s\n", p);
        result.push_back(p);
        p=strtok(NULL, delim);
    }
    return result;
}

std::string extractVersion(const std::string& instr) {
    std::regex pattern(R"((\d+)(?:[._](\d+))?(?:[._](\d+))?(?:[._](\d+))?)");
    std::smatch matches;

    if (std::regex_search(instr, matches, pattern)) {
        std::string majorVersion = matches[1].str();
        std::string minorVersion = matches[2].str();
        std::string patchVersion = matches[3].str();
        std::string flagVersion = matches[4].str();

        std::string version = majorVersion;
        if (!minorVersion.empty())
            version += "." + minorVersion;
        if (!patchVersion.empty())
            version += "." + patchVersion;
        if (!flagVersion.empty())
            version += "." + flagVersion;

        return version;
    }

    return "";
}


std::vector<int> splitVersion(const std::string& version) {
    std::vector<int> result;
    std::string current;

    for (char c : version) {
        if (c == '.' || c == '_') {
            if (!current.empty()) {
                result.push_back(std::stoi(current));
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        result.push_back(std::stoi(current));
    }

    return result;
}

int compareVersion(const std::string& version1, const std::string& version2)
{
    std::vector<int> v1 = splitVersion(version1);
    std::vector<int> v2 = splitVersion(version2);

    int n = std::max(v1.size(), v2.size());

    for (int i = 0; i < n; i++) {
        int num1 = (i < v1.size()) ? v1[i] : 0;
        int num2 = (i < v2.size()) ? v2[i] : 0;

        if (num1 < num2) {
            return -1;
        } else if (num1 > num2) {
            return 1;
        }
    }

    return 0;
}
