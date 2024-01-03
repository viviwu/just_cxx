//
// Created by viviwu on 1/3/24.
//

#include <iostream>
#include <memory>

class MyClass {
 public:
  MyClass() { std::cout << "Constructor called." << std::endl; }

  ~MyClass() { std::cout << "Destructor called." << std::endl; }
};

void demoUniquePtr() {
  // 使用 std::unique_ptr
  std::cout << "std::unique_ptr demo:" << std::endl;
  std::unique_ptr<MyClass> uniquePtr(new MyClass());

  // std::unique_ptr 不支持所有权转移，下面的语句会导致编译错误
  // std::unique_ptr<MyClass> anotherPtr = uniquePtr;

  // 使用 std::move 进行所有权转移
  std::unique_ptr<MyClass> anotherPtr = std::move(uniquePtr);

  if (uniquePtr) {
    std::cout << "uniquePtr is not null." << std::endl;
  } else {
    std::cout << "uniquePtr is null." << std::endl;
  }
}

void demoSharedPtr() {
  // 使用 std::shared_ptr
  std::cout << "std::shared_ptr demo:" << std::endl;

  std::shared_ptr<MyClass> sharedPtr1(new MyClass());
  std::shared_ptr<MyClass> sharedPtr2 = sharedPtr1;

  std::cout << "sharedPtr1 count: " << sharedPtr1.use_count() << std::endl;
  std::cout << "sharedPtr2 count: " << sharedPtr2.use_count() << std::endl;
}

void demoWeakPtr() {
  // 使用 std::weak_ptr
  std::cout << "std::weak_ptr demo:" << std::endl;

  std::shared_ptr<MyClass> sharedPtr(new MyClass());
  std::weak_ptr<MyClass> weakPtr = sharedPtr;

  std::cout << "sharedPtr count: " << sharedPtr.use_count() << std::endl;
  std::cout << "weakPtr expired: " << weakPtr.expired() << std::endl;
  if (weakPtr.expired()) {
    std::cout << "The observed object has been destroyed." << std::endl;
  } else {
    std::cout << "The observed object still exists." << std::endl;
  }

  // 使用 std::lock_weak
  std::shared_ptr<MyClass> sharedPtr2 = weakPtr.lock();
  if (sharedPtr2) {
    std::cout << "sharedPtr2 count: " << sharedPtr2.use_count() << std::endl;
  } else {
    std::cout << "weakPtr is expired." << std::endl;
  }
}

int main() {
  demoUniquePtr();
  std::cout << std::endl;

  demoSharedPtr();
  std::cout << std::endl;

  demoWeakPtr();
  std::cout << std::endl;

  return 0;
}