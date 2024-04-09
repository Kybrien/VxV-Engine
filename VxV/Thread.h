#pragma once
#include <thread>
#include <mutex>
#include <functional>

class Thread {
public:
public:
    static void createThread(std::function<void()> func, int id);

    class Mutex {
    public:
        Mutex();
        void lock();
        void unlock();

    private:
        std::mutex mutex;
    };

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
};
