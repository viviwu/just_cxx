//
// Created by viviwu on 2023/5/25.
//

#include <iostream>
#include <regex>
#include <string>

uint32_t get_local_date()
{
    time_t local_time;
    struct tm * timeinfo;
    time(&local_time);
    timeinfo = localtime(&local_time);
    return
        (timeinfo->tm_year + 1900) * 10000 +
        (timeinfo->tm_mon + 1) * 100 +
        timeinfo->tm_mday;
}

template< typename... Args >
std::string str_format(const char* format, Args... args) {
    int length = std::snprintf(nullptr, 0, format, args...);
    char* buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);
    std::string str(buf);
    delete[] buf;
    return str;
}

int main() {
    auto today = get_local_date();
    std::string pattern = str_format(".*ReportOrderAlgo_%d.dbf", today);
    std::cout << "pattern: " << pattern << std::endl;
    std::regex order_algo_file_rgx(pattern);
    std::string file = "C:/Users/Administrator/Desktop/FileTrading/ATGO/ReportOrderAlgo_20230524.dbf";
    std::cmatch what;
    if (std::regex_match(file.c_str(), what, order_algo_file_rgx)) {
        std::cout << file << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    getchar();
    return 0;
}
