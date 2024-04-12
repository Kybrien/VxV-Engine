#include "Thread.h"

std::vector<std::thread> Thread::threads;

void Thread::createThread(std::function<void()> func, ThreadId id) {
    threads.emplace_back(func);
}

void Thread::joinAll() {
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
