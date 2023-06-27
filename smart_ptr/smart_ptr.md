C++11 引入了智能指针（smart pointer），它是一个类模板，可以自动管理指针的生命周期，避免内存泄漏等问题。C++标准库提供了三种智能指针：std::unique_ptr、std::shared_ptr 和 std::weak_ptr。

1. std::unique_ptr

std::unique_ptr 是一种独占式智能指针，用于管理独占所有权的动态分配的对象。它保证了对象在所有权范围之外被自动删除，避免了内存泄漏的问题。std::unique_ptr 可以用于以下情况：

- 管理动态分配的单个对象；
- 在容器中管理动态分配的对象。

std::unique_ptr 的使用方法：

```cpp
std::unique_ptr<int> ptr1(new int(10));  // 创建一个智能指针
std::unique_ptr<int> ptr2 = std::move(ptr1);  // 移动构造函数
std::unique_ptr<int> ptr3 = std::make_unique<int>(20);  // 使用 make_unique 创建智能指针
```

2. std::shared_ptr

std::shared_ptr 是一种共享式智能指针，用于管理共享所有权的动态分配的对象。它可以被多个指针共享，每个指针都有一个计数器来记录有多少个指针共享这个对象。当计数器为 0 时，自动删除所管理的对象。std::shared_ptr 可以用于以下情况：

- 管理动态分配的单个对象；
- 在容器中管理动态分配的对象；
- 在多线程中共享对象。

std::shared_ptr 的使用方法：

```cpp
std::shared_ptr<int> ptr1(new int(10));  // 创建一个智能指针
std::shared_ptr<int> ptr2 = ptr1;  // 复制构造函数
std::shared_ptr<int> ptr3 = std::make_shared<int>(20);  // 使用 make_shared 创建智能指针
```

3. std::weak_ptr

std::weak_ptr 是一种弱引用式智能指针，它指向由 std::shared_ptr 管理的对象，但不增加引用计数。它可以用于解决 std::shared_ptr 的循环引用问题，避免了内存泄漏问题。std::weak_ptr 可以用于以下情况：

- 在 std::shared_ptr 的循环引用时使用。

std::weak_ptr 的使用方法：

```cpp
std::shared_ptr<int> shared = std::make_shared<int>(10);
std::weak_ptr<int> weak = shared;  // 创建一个弱引用式智能指针
std::shared_ptr<int> shared2 = weak.lock();  // 转换为 std::shared_ptr
```

需要注意的是，智能指针并不是万能的，它只能自动管理动态分配的内存，无法管理栈上的对象。此外，在使用智能指针时，还需要注意防止循环引用等问题，以免造成内存泄漏。

***
***

使用 std::make_unique 和 std::make_shared 时，建议遵循以下最佳实践：

1. 使用 std::make_unique 来创建 std::unique_ptr

使用 std::make_unique 可以避免显式地使用 new 运算符，避免内存泄漏和异常情况下的内存泄漏。例如：

```cpp
std::unique_ptr<MyObject> obj = std::make_unique<MyObject>();
```

2. 使用 std::make_shared 来创建 std::shared_ptr

使用 std::make_shared 可以避免显式地使用 new 运算符，避免内存泄漏和异常情况下的内存泄漏。此外，使用 std::make_shared 还可以减少内存分配和引用计数的开销，因为它将对象和引用计数一起分配到同一块内存中。例如：

```cpp
std::shared_ptr<MyObject> obj = std::make_shared<MyObject>();
```

3. 不要在 std::make_unique 和 std::make_shared 的参数中传递动态分配的对象

在使用 std::make_unique 和 std::make_shared 时，不要在其参数中传递动态分配的对象，因为这样会导致内存泄漏。例如：

```cpp
// 不要这样做
std::unique_ptr<MyObject> obj1(std::unique_ptr<MyObject>(new MyObject));
std::shared_ptr<MyObject> obj2(std::shared_ptr<MyObject>(new MyObject));
```

应该这样做：

```cpp
std::unique_ptr<MyObject> obj1 = std::make_unique<MyObject>();
std::shared_ptr<MyObject> obj2 = std::make_shared<MyObject>();
```

4. 在需要传递 std::unique_ptr 或 std::shared_ptr 的函数中，使用 std::move

在需要传递 std::unique_ptr 或 std::shared_ptr 的函数中，使用 std::move 可以避免不必要的内存复制和引用计数增加。例如：

```cpp
void myFunc(std::unique_ptr<MyObject> obj);

std::unique_ptr<MyObject> obj = std::make_unique<MyObject>();
myFunc(std::move(obj));
```

```cpp
void myFunc(std::shared_ptr<MyObject> obj);

std::shared_ptr<MyObject> obj = std::make_shared<MyObject>();
myFunc(std::move(obj));
```

总之，使用 std::make_unique 和 std::make_shared 可以避免内存泄漏和异常情况下的内存泄漏，并且可以提高代码的可读性和可维护性。在使用 std::make_unique 和 std::make_shared 时，需要注意遵循最佳实践，避免滥用和不必要的开销。