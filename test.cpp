#include <iostream>

#include "threadpool.hpp"

int main() {
    concurrent::threadpool tp(4);

    (void) tp.enqueue_r([]() { std::cout << "hello world!" << std::endl; });

    auto r1 = tp.enqueue([](int answer) { return answer; }, 1);
    std::cout << r1.get() << std::endl;

    auto r2 = tp.enqueue_r([](int answer) { std::cout << "yay, blocked on shutdown." << std::endl; return answer; }, 2);
    (void) r2;

    (void) tp.enqueue([]() { throw std::runtime_error("why this function got called?!"); });

    int x = 0;
    auto r3 = tp.enqueue([&](){ x = 1; });
    r3.get();
    if (x != 1) throw std::runtime_error("why x not equals to 1?!");

    (void) tp.enqueue_r([=]() mutable { x = 2; });
    if (x != 1) throw std::runtime_error("why x not equals to 1?!");

    return 0;
}

