/**
 ******************************************************************************
 * @file           : test_protobuf.cpp.c
 * @author         : viviwu
 * @brief          : None
 * @attention      : None
 * @date           : 1/27/24
 ******************************************************************************
 */

#include <iostream>
#include "test_msg.pb.h"

int main() {
  // 创建一个 Person 对象
  Person person;
  person.set_name("Alice");
  person.set_age(25);

  // 将 Person 对象序列化为字节流
  std::string serialized_data = person.SerializeAsString();

  // 反序列化字节流为 Person 对象
  Person deserialized_person;
  deserialized_person.ParseFromString(serialized_data);

  // 打印 Person 对象的字段
  std::cout << "Name: " << deserialized_person.name() << std::endl;
  std::cout << "Age: " << deserialized_person.age() << std::endl;

  return 0;
}