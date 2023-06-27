C++11 ����������ָ�루smart pointer��������һ����ģ�壬�����Զ�����ָ����������ڣ������ڴ�й©�����⡣C++��׼���ṩ����������ָ�룺std::unique_ptr��std::shared_ptr �� std::weak_ptr��

1. std::unique_ptr

std::unique_ptr ��һ�ֶ�ռʽ����ָ�룬���ڹ����ռ����Ȩ�Ķ�̬����Ķ�������֤�˶���������Ȩ��Χ֮�ⱻ�Զ�ɾ�����������ڴ�й©�����⡣std::unique_ptr �����������������

- ����̬����ĵ�������
- �������й���̬����Ķ���

std::unique_ptr ��ʹ�÷�����

```cpp
std::unique_ptr<int> ptr1(new int(10));  // ����һ������ָ��
std::unique_ptr<int> ptr2 = std::move(ptr1);  // �ƶ����캯��
std::unique_ptr<int> ptr3 = std::make_unique<int>(20);  // ʹ�� make_unique ��������ָ��
```

2. std::shared_ptr

std::shared_ptr ��һ�ֹ���ʽ����ָ�룬���ڹ���������Ȩ�Ķ�̬����Ķ��������Ա����ָ�빲��ÿ��ָ�붼��һ������������¼�ж��ٸ�ָ�빲��������󡣵�������Ϊ 0 ʱ���Զ�ɾ��������Ķ���std::shared_ptr �����������������

- ����̬����ĵ�������
- �������й���̬����Ķ���
- �ڶ��߳��й������

std::shared_ptr ��ʹ�÷�����

```cpp
std::shared_ptr<int> ptr1(new int(10));  // ����һ������ָ��
std::shared_ptr<int> ptr2 = ptr1;  // ���ƹ��캯��
std::shared_ptr<int> ptr3 = std::make_shared<int>(20);  // ʹ�� make_shared ��������ָ��
```

3. std::weak_ptr

std::weak_ptr ��һ��������ʽ����ָ�룬��ָ���� std::shared_ptr ����Ķ��󣬵����������ü��������������ڽ�� std::shared_ptr ��ѭ���������⣬�������ڴ�й©���⡣std::weak_ptr �����������������

- �� std::shared_ptr ��ѭ������ʱʹ�á�

std::weak_ptr ��ʹ�÷�����

```cpp
std::shared_ptr<int> shared = std::make_shared<int>(10);
std::weak_ptr<int> weak = shared;  // ����һ��������ʽ����ָ��
std::shared_ptr<int> shared2 = weak.lock();  // ת��Ϊ std::shared_ptr
```

��Ҫע����ǣ�����ָ�벢�������ܵģ���ֻ���Զ�����̬������ڴ棬�޷�����ջ�ϵĶ��󡣴��⣬��ʹ������ָ��ʱ������Ҫע���ֹѭ�����õ����⣬��������ڴ�й©��

***
***

ʹ�� std::make_unique �� std::make_shared ʱ��������ѭ�������ʵ����

1. ʹ�� std::make_unique ������ std::unique_ptr

ʹ�� std::make_unique ���Ա�����ʽ��ʹ�� new ������������ڴ�й©���쳣����µ��ڴ�й©�����磺

```cpp
std::unique_ptr<MyObject> obj = std::make_unique<MyObject>();
```

2. ʹ�� std::make_shared ������ std::shared_ptr

ʹ�� std::make_shared ���Ա�����ʽ��ʹ�� new ������������ڴ�й©���쳣����µ��ڴ�й©�����⣬ʹ�� std::make_shared �����Լ����ڴ��������ü����Ŀ�������Ϊ������������ü���һ����䵽ͬһ���ڴ��С����磺

```cpp
std::shared_ptr<MyObject> obj = std::make_shared<MyObject>();
```

3. ��Ҫ�� std::make_unique �� std::make_shared �Ĳ����д��ݶ�̬����Ķ���

��ʹ�� std::make_unique �� std::make_shared ʱ����Ҫ��������д��ݶ�̬����Ķ�����Ϊ�����ᵼ���ڴ�й©�����磺

```cpp
// ��Ҫ������
std::unique_ptr<MyObject> obj1(std::unique_ptr<MyObject>(new MyObject));
std::shared_ptr<MyObject> obj2(std::shared_ptr<MyObject>(new MyObject));
```

Ӧ����������

```cpp
std::unique_ptr<MyObject> obj1 = std::make_unique<MyObject>();
std::shared_ptr<MyObject> obj2 = std::make_shared<MyObject>();
```

4. ����Ҫ���� std::unique_ptr �� std::shared_ptr �ĺ����У�ʹ�� std::move

����Ҫ���� std::unique_ptr �� std::shared_ptr �ĺ����У�ʹ�� std::move ���Ա��ⲻ��Ҫ���ڴ渴�ƺ����ü������ӡ����磺

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

��֮��ʹ�� std::make_unique �� std::make_shared ���Ա����ڴ�й©���쳣����µ��ڴ�й©�����ҿ�����ߴ���Ŀɶ��ԺͿ�ά���ԡ���ʹ�� std::make_unique �� std::make_shared ʱ����Ҫע����ѭ���ʵ�����������úͲ���Ҫ�Ŀ�����