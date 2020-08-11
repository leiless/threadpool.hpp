# threadpool.hpp

`threadpool.hpp` is a simple header-only thread pool implementation in C++17.

No any extra dependency is required, easy to use.

## API

### Initialize a threadpool

```c++
#include "threadpool.hpp"

constexpr int THREADPOOL_SIZE = 4;
concurrent::threadpool tp(THREADPOOL_SIZE);
```

### Enqueue a task by given function and its arguments

```c++
template<typename Fn, typename... Args>
decltype(auto) enqueue(Fn && fn, Args &&... args);
```

### Enqueue a task(block on shutdown)

```c++
template<typename Fn, typename... Args>
decltype(auto) enqueue_r(Fn && fn, Args &&... args);
```

Same as `enqueue()`, yet the task will still be executed even if the thread pool is shutting down instead of simply discarded.

## Example

```c++
#include <iostream>
#include "threadpool.hpp"

int main() {
    constexpr int THREADPOOL_SIZE = 2;
    concurrent::threadpool tp(THREADPOOL_SIZE);
    tp.enqueue_r([]() { std::cout << "hello world" << std::endl; });
    return 0;
}
```

More checkout [`test.cpp`](test.cpp).

## Courtesy

This implementation mainly inspired by [progschj/ThreadPool](https://github.com/progschj/ThreadPool) and [Build Your Own Threadpool With C++](https://kingsamchen.github.io/2018/05/30/build-your-own-thread-pool-with-cpp/).

