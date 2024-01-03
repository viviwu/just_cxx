//
// Created by viviwu on 1/3/24.
//

#include <iostream>
#include <memory>

class MyClass: public std::enable_shared_from_this<MyClass>  {
 public:
  MyClass() { std::cout << "Constructor called." << std::endl; }

  ~MyClass() { std::cout << "Destructor called." << std::endl; }

  void doSomething() {
    //enable_shared_from_this first
    std::shared_ptr<MyClass> sharedPtr = shared_from_this();
    std::cout << "Doing something with sharedPtr" << std::endl;
    // 在这里可以安全地使用 sharedPtr，确保对象的有效性
  }
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
  sharedPtr2->doSomething();
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
    weakPtr.lock()->doSomething();
  }

  // 使用 std::lock_weak
  std::shared_ptr<MyClass> sharedPtr2 = weakPtr.lock();
  if (sharedPtr2) {
    std::cout << "sharedPtr2 count: " << sharedPtr2.use_count() << std::endl;
  } else {
    std::cout << "weakPtr is expired." << std::endl;
  }

}

void weak_ptr_lock() {
  std::shared_ptr<int> sharedPtr(new int(42));
  std::weak_ptr<int> weakPtr(sharedPtr);
  std::cout << "std::weak_ptr lock() get():" <<*(weakPtr.lock().get())<< std::endl;
  sharedPtr.reset();

  std::shared_ptr<int> lockedPtr = weakPtr.lock();
  if (lockedPtr) {
    std::cout << "The observed object still exists." << std::endl;
    std::cout << "Value: " << *lockedPtr << std::endl;
  } else {
    std::cout << "The observed object has been destroyed." << std::endl;
  }

}

int main() {
  demoUniquePtr();
  std::cout << std::endl;

  demoSharedPtr();
  std::cout << std::endl;

  demoWeakPtr();
  std::cout << std::endl;

  weak_ptr_lock();

  return 0;
}