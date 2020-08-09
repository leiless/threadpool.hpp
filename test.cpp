#include <iostream>

#include "threadpool.hpp"

int main() {
    concurrent::threadpool tp(2);

    tp.enqueue_discard_r([]() { std::cout << "hello world!" << std::endl; });

    auto r1 = tp.enqueue([](int answer) { return answer; }, 1);
    std::cout << r1.get() << std::endl;

    auto r2 = tp.enqueue_r([](int answer) { std::cout << "yay, blocked on shutdown." << std::endl; return answer; }, 2);
    (void) r2;

    tp.enqueue_discard([]() { throw std::runtime_error("why this function got called?!"); });

    return 0;
}

