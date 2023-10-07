C++ STL �е��㷨�ṩ��һ������õĺ����������ڸ������ݽṹ�Ͻ��в�������������������ȡ���Щ�㷨�Ƿ��͵ģ�����Ӧ���ڸ������͵����ݣ������͡������͡��Զ������͵ȡ�STL �㷨�㷺Ӧ���ڸ���Ӧ�ó����У�������ֵ���㡢���ݷ�����ͼ��ͼ���������

������һЩ���õ� STL �㷨����Ӧ�ã�

1. std::sort

std::sort �㷨���ڶ������е�Ԫ�ؽ����������������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ�std::sort �㷨���ÿ��������㷨ʵ�֣������� O(nlogn) ��ʱ�临�Ӷȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::sort(vec.begin(), vec.end());
```

2. std::for_each

std::for_each �㷨���ڶ������е�ÿ��Ԫ��ִ��һ�����������������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::for_each(vec.begin(), vec.end(), [](int i) { std::cout << i << " "; });
```

3. std::accumulate

std::accumulate �㷨���ڼ�������������Ԫ�ص��ܺ͡����������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
int sum = std::accumulate(vec.begin(), vec.end(), 0);
```

4. std::transform

std::transform �㷨���ڶ������е�ÿ��Ԫ��ִ��һ�����������������Ľ���洢����һ�������С����������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::vector<int> vec2(vec.size());
std::transform(vec.begin(), vec.end(), vec2.begin(), [](int i) { return i * 2; });
```

5. std::find

std::find �㷨�����������в���ĳ��Ԫ���Ƿ���ڡ����������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
auto it = std::find(vec.begin(), vec.end(), 5);
if (it != vec.end())
{
    std::cout << "5 found at position " << std::distance(vec.begin(), it) << std::endl;
}
```

6. std::copy

std::copy �㷨���ڽ�һ�������е�Ԫ�ظ��Ƶ���һ�������С����������ڴ󲿷� STL ���������� std::vector��std::list��std::deque �ȡ����磺

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::vector<int> vec2(vec.size());
std::copy(vec.begin(), vec.end(), vec2.begin());
```

��֮��C++ STL �е��㷨�ṩ��һ������õĺ����������ڸ������ݽṹ�Ͻ��в�������Щ�㷨�Ƿ��͵ģ�����Ӧ���ڸ������͵����ݣ����Լ��ٴ���������ߴ���Ŀɶ��ԺͿ�ά���ԡ���Щ�㷨�㷺Ӧ���ڸ���Ӧ�ó����У�������ֵ���㡢���ݷ�����ͼ��ͼ���������