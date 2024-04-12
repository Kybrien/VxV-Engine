#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>

enum class ThreadCategory {
    UI,
    Network,
    Graphics
};

struct ThreadId {
    ThreadCategory category;
    int subThreadId;
};

class Thread {

private:
    static std::vector<std::thread> threads;
public:
    static void createThread(std::function<void()> func, ThreadId id);
    static void joinAll();

    class Mutex {
    public:
        Mutex();
        void lock();
        void unlock();

    private:
        std::mutex mutex;
    };
    const std::vector<std::thread>& getThreads() {
        return threads;
    }

    class LockGuard {
    public:
        LockGuard(Mutex& mutex);

        // Destructeur
        ~LockGuard();

    private:
        Mutex& mutex;
    };

    class ConditionVariable {
    public:
        void wait(Mutex& mutex);

        void notifyOne();
        void notifyAll();

    private:
        std::condition_variable conditionVariable;
    };

    template<typename T>
    class Atomic {
    public:
        Atomic(T value) : atomicValue(value) {}

        void set(T value) {
            atomicValue.store(value);
        }

        T get() {
            return atomicValue.load();
        }

    private:
        std::atomic<T> atomicValue;
    };
};
