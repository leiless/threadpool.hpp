/*
 * Trivial header-only thread pool implementation in modern C++.
 *
 * Created:     Aug 9, 2020.
 * Repository:  https://github.com/leiless/threadpool.hpp
 * LICENSE:     BSD-2-Clause
 */

#ifndef THE_THREADPOOL_HPP
#define THE_THREADPOOL_HPP

#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <future>

#define NAMESPACE_BEGIN(name)       namespace name {
#define NAMESPACE_END()             }

NAMESPACE_BEGIN(concurrent)

class threadpool {
public:
    explicit threadpool(size_t threads) :alive(true) {
        if (threads == 0) {
            throw std::runtime_error("thread pool size cannot be zero");
        }
        for (auto i = 0llu; i < threads; i++) {
            workers.emplace_back([this] { worker_main(); });
        }
    }

    ~threadpool() noexcept {
        {
            std::unique_lock<decltype(mtx)> lock(mtx);
            alive = false;
        }
        cv.notify_all();
        for (auto & worker : workers) {
            worker.join();
        }
    }

    template<typename Fn, typename... Args>
    decltype(auto) enqueue(bool block_on_shutdown, Fn && fn, Args &&... args) {
        using return_type = std::invoke_result_t<Fn, Args...>;

        auto t1 = std::packaged_task<return_type()>(
            std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...)
        );
        auto future = t1.get_future();
        auto t2 = std::make_pair(std::move(t1), block_on_shutdown);

        {
            std::lock_guard<decltype(mtx)> lock(mtx);
            if (!alive) {
                throw std::runtime_error("enqueue on stopped thread pool");
            }
            tasks.push(std::move(t2));
        }

        cv.notify_one();
        return future;
    }

private:
    using task = std::pair<std::function<void()>, bool>;

    inline task task_dequeue() noexcept {
        task t;

        std::unique_lock<decltype(mtx)> lock(mtx);
        cv.wait(lock, [this] { return !tasks.empty() || !alive; });

        while (!tasks.empty()) {
            if (!alive && !tasks.front().second) {
                tasks.pop();
                continue;
            }

            t = std::move(tasks.front());
            tasks.pop();
            break;
        }

        return t;
   }

    void worker_main() {
        while (true) {
            task t = task_dequeue();
            // The thread pool is going to shutdown
            if (t.first == nullptr) break;
            t.first();
        }
    }

    bool alive;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<task> tasks;
    std::vector<std::thread> workers;
};

NAMESPACE_END()

#endif

