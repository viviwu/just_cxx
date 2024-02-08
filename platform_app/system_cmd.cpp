/**
  ******************************************************************************
  * @file           : system_cmd.cpp.c
  * @author         : wuxw
  * @brief          : None
  ******************************************************************************
*/

#include <iostream>
#include <cstdlib>

int main() {
  // create symbolic link:
  // const char* command = "mklink /D D:\\link_to_folder2 D:\\folder\\folder2";

  const char* command = "echo %path%";
  std::cout << command << std::endl;
  int result = std::system(command);

  // Handle the result if needed
  if (result == 0) {
    std::cout << "CMD executed successfully." << std::endl;
  } else {
    std::cerr << "Failed to execute CMD." << command << std::endl;
  }
  return 0;
}
