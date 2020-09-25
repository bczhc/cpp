#ifndef BCZHC_CONCURRENT_H
#define BCZHC_CONCURRENT_H

#include "./third_party/practice/LinearList.hpp"
#include <cstdio>
#include <cstdlib>
#include <features.h>
#include <pthread.h>
#include <sched.h>

using namespace bczhc;
using namespace linearlist;

namespace bczhc {
namespace concurrent {
class MutexLock {
public:
    class Condition {
    private:
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        pthread_mutex_t &mutex;

    public:
        Condition(MutexLock &mutexLock) : mutex(mutexLock.mutexLock) {}

        ~Condition() { pthread_cond_destroy(&cond); }

        inline void wait() { pthread_cond_wait(&cond, &mutex); }

        inline void signal() { pthread_cond_signal(&cond); }
    };

    pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;

    ~MutexLock() { pthread_mutex_destroy(&mutexLock); }

    inline void lock() { pthread_mutex_lock(&mutexLock); };

    inline void unlock() { pthread_mutex_unlock(&mutexLock); }

    inline bool tryLock() { return pthread_mutex_trylock(&mutexLock) == 0; }

    Condition newCondition() {
        Condition cond(*this);
        return cond;
    }

private:
    Condition mCond = newCondition();

public:
    inline void wait() { mCond.wait(); }

    inline void notify() { mCond.signal(); }
};

class CountDownLatch {
private:
    int count;

public:
    CountDownLatch(int count) : count(count) {}
    CountDownLatch() {}
    void await() {
        while (count != 0) {
        }
    }

    void countDown() { --count; }

    void set(int count) { this->count = count; }
};

class Runnable {
public:
    virtual void run() = 0;
};

template <typename ArgType> class Consumer {
public:
    virtual void accept(ArgType &arg) = 0;
};

void *call(void *arg) {
    Runnable *runnable = (Runnable *)arg;
    runnable->run();
    return nullptr;
}

template <typename T> struct Bean {
    Consumer<T> *c;
    void *arg;
};

template <typename ArgType> class ConsumerCall {
public:
    static void consume(ArgType &arg) {}

    static void *call(void *arg) {
        Bean<ArgType> &b = *(Bean<ArgType> *)arg;
        ((Consumer<ArgType> *)b.c)->accept(*(ArgType *)b.arg);
        delete &b;
        return nullptr;
    }
};

class Thread {
private:
    pthread_t t;

public:
    Thread(Runnable *runnable) { pthread_create(&t, nullptr, call, runnable); }

    void join() { pthread_join(t, nullptr); }
};

class ThreadPool {
public:
    virtual void execute(Runnable *r) = 0;

    ~ThreadPool() {}
};

class Executors {
public:
    class FixedThreadPool : public ThreadPool {
    private:
        MutexLock lock1;
        MutexLock lock2;
        class CoreThreadRunnable : public Runnable {
        private:
            Queue<Runnable *> &runnables;
            MutexLock &lock1;
            MutexLock &lock2;

        public:
            CoreThreadRunnable(Queue<Runnable *> &runnables, MutexLock &lock1,
                               MutexLock &lock2)
                : runnables(runnables), lock1(lock1), lock2(lock2) {}
            void run() override {
                while (true) {
                    while (runnables.isEmpty()) {
                        lock1.lock();
                        lock1.wait();
                        lock1.unlock();
                    }
                    lock2.lock();
                    Runnable *runnable = runnables.dequeue();
                    lock2.unlock();
                    runnable->run();
                }
            }
        };
        Queue<Runnable *> runnables;
        Thread **coreThreads;
        int poolSize;
        CoreThreadRunnable *coreThreadRunnable;

    public:
        FixedThreadPool(int poolSize) {
            this->poolSize = poolSize;
            coreThreads = new Thread *[poolSize];
            coreThreadRunnable =
                new CoreThreadRunnable(runnables, lock1, lock2);
            for (int i = 0; i < poolSize; ++i) {
                coreThreads[i] = new Thread(coreThreadRunnable);
            }
        }

        ~FixedThreadPool() {
            for (int i = 0; i < poolSize; ++i) {
                delete coreThreads[i];
            }
            delete coreThreads;
            delete coreThreadRunnable;
        }

        void execute(Runnable *runnable) override {
            lock2.lock();
            runnables.enqueue(runnable);
            lock1.lock();
            lock1.notify();
            lock1.unlock();
            lock2.unlock();
        }
    };
    static ThreadPool *newFixedThreadPool(int poolSize) {
        return new FixedThreadPool(poolSize);
    }
};
} // namespace concurrent
} // namespace bczhc

#endif // BCZHC_CONCURRENT_H