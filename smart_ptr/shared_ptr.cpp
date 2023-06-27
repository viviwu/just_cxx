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
�����ʾ���У�MyClass �̳��� std::enable_shared_from_this<MyClass>������ζ�� MyClass ��ʵ������ͨ�� shared_from_this() ��������һ�� std::shared_ptr ������� std::shared_ptr ������ MyClass ��ʵ����������Ȩ��

�� MyClass::getShared() �����У����� shared_from_this() ��������һ�� std::shared_ptr ������� std::shared_ptr ������ p1 ���� MyClass ������Ȩ��

��Ҫע����ǣ�Ϊ��ʹ�� std::enable_shared_from_this������ʹ�� std::shared_ptr ���������� MyClass ��ʵ�������ʹ���������͵�����ָ�룬�� std::unique_ptr�����޷�ʹ�� std::enable_shared_from_this��

ʹ�� std::enable_shared_from_this ���Ա�����������ʽ���� std::shared_ptr ������鷳��ʹ������Ӽ���׶�������Ҫע����ǣ�ʹ�� std::enable_shared_from_this ʱ������Ҫ��֤����ʼ���Ǳ� std::shared_ptr ��������������ᵼ��δ�������Ϊ��
 */