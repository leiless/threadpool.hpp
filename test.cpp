#include <iostream>

#include "threadpool.hpp"

int main() {
    concurrent::threadpool tp(2);
    tp.enqueue([](){ std::cout << "hello world!" << std::endl; });
    auto r = tp.enqueue(false, [](int answer) { return answer; }, 1);
    std::cout << r.get() << std::endl;
    return 0;
}

