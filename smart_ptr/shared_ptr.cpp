//
// Created by dejavu on 2023/6/12.
//
#include <memory>
#include <iostream>


class MyClass : public std::enable_shared_from_this<MyClass>
{
public:
    MyClass(){
        std::cout<<__FUNCTION__ <<std::endl;
    }

    ~MyClass(){
        std::cout<<__FUNCTION__ <<std::endl;
    }

    std::shared_ptr<MyClass> getShared()
    {
        return shared_from_this();
    }
};

int main()
{
//    std::shared_ptr<MyClass> p1(new MyClass);
    std::shared_ptr<MyClass> p1=std::make_shared<MyClass>();
    std::shared_ptr<MyClass> p2 = p1->getShared();

    return 0;
}

/*
在这个示例中，MyClass 继承自 std::enable_shared_from_this<MyClass>，这意味着 MyClass 的实例可以通过 shared_from_this() 函数返回一个 std::shared_ptr 对象，这个 std::shared_ptr 对象与 MyClass 的实例共享所有权。

在 MyClass::getShared() 函数中，调用 shared_from_this() 函数返回一个 std::shared_ptr 对象，这个 std::shared_ptr 对象与 p1 共享 MyClass 的所有权。

需要注意的是，为了使用 std::enable_shared_from_this，必须使用 std::shared_ptr 对象来管理 MyClass 的实例。如果使用其他类型的智能指针，如 std::unique_ptr，将无法使用 std::enable_shared_from_this。

使用 std::enable_shared_from_this 可以避免在类中显式传递 std::shared_ptr 对象的麻烦，使代码更加简洁易读。但需要注意的是，使用 std::enable_shared_from_this 时，必须要保证该类始终是被 std::shared_ptr 对象所管理，否则会导致未定义的行为。
 */