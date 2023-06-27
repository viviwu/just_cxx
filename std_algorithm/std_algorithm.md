C++ STL 中的算法提供了一组可重用的函数，可以在各种数据结构上进行操作，例如容器、数组等。这些算法是泛型的，可以应用于各种类型的数据，如整型、浮点型、自定义类型等。STL 算法广泛应用于各种应用程序中，包括数值计算、数据分析、图形图像处理等领域。

下面是一些常用的 STL 算法及其应用：

1. std::sort

std::sort 算法用于对容器中的元素进行排序。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。std::sort 算法采用快速排序算法实现，它具有 O(nlogn) 的时间复杂度。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::sort(vec.begin(), vec.end());
```

2. std::for_each

std::for_each 算法用于对容器中的每个元素执行一个操作。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::for_each(vec.begin(), vec.end(), [](int i) { std::cout << i << " "; });
```

3. std::accumulate

std::accumulate 算法用于计算容器中所有元素的总和。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
int sum = std::accumulate(vec.begin(), vec.end(), 0);
```

4. std::transform

std::transform 算法用于对容器中的每个元素执行一个操作，并将操作的结果存储到另一个容器中。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::vector<int> vec2(vec.size());
std::transform(vec.begin(), vec.end(), vec2.begin(), [](int i) { return i * 2; });
```

5. std::find

std::find 算法用于在容器中查找某个元素是否存在。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
auto it = std::find(vec.begin(), vec.end(), 5);
if (it != vec.end())
{
    std::cout << "5 found at position " << std::distance(vec.begin(), it) << std::endl;
}
```

6. std::copy

std::copy 算法用于将一个容器中的元素复制到另一个容器中。它可以用于大部分 STL 容器，包括 std::vector、std::list、std::deque 等。例如：

```cpp
std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
std::vector<int> vec2(vec.size());
std::copy(vec.begin(), vec.end(), vec2.begin());
```

总之，C++ STL 中的算法提供了一组可重用的函数，可以在各种数据结构上进行操作。这些算法是泛型的，可以应用于各种类型的数据，可以减少代码量，提高代码的可读性和可维护性。这些算法广泛应用于各种应用程序中，包括数值计算、数据分析、图形图像处理等领域。