#include "Concurrent.h"

using namespace bczhc;
using namespace concurrent;

MutexLock::Condition::~Condition() {
    pthread_cond_destroy(&cond);
}

void MutexLock::Condition::wait() { pthread_cond_wait(&cond, &mutex); }

void MutexLock::Condition::signal() {
    pthread_cond_signal(&cond);
}


MutexLock::~MutexLock() {
    pthread_mutex_destroy(&mutexLock);
}

void MutexLock::lock() {
    pthread_mutex_lock(&mutexLock);
}

void MutexLock::unlock() {
    pthread_mutex_unlock(&mutexLock);
}

bool MutexLock::tryLock() {
    return pthread_mutex_trylock(&mutexLock) == 0;
}

MutexLock::Condition MutexLock::newCondition() {
    Condition cond(*this);
    return cond;
}

void MutexLock::wait() { mCond.wait(); }

void MutexLock::notify() { mCond.signal(); }

void CountDownLatch::await() const {
    while (count != 0) {
    }
}

void CountDownLatch::countDown() {
    lock.lock();
    --count;
    lock.unlock();
}

CountDownLatch::CountDownLatch() = default;

void CountDownLatch::set(int _count) {
    lock.lock();
    this->count = _count;
    lock.unlock();
}

LongWaitCountDownLatch::LongWaitCountDownLatch(int count) : count(count) {}

void LongWaitCountDownLatch::countDown() {
    lock.lock();
    if (--count == 0) {
        lock.notify();
    }
    lock.unlock();
}

void LongWaitCountDownLatch::wait() {
    lock.lock();
    lock.wait();
    lock.unlock();
}

void LongWaitCountDownLatch::interruptAndReset() {
    lock.lock();
    count = 0;
    lock.notify();
    lock.unlock();
}

Thread::Thread(Runnable *runnable) { pthread_create(&t, nullptr, call, runnable); }

void Thread::join() const { pthread_join(t, nullptr); }

void Thread::sleep(int64_t millis) {
    usleep(millis * 1000);
}

ThreadPool::~ThreadPool() = default;

Executors::FixedThreadPool::CoreThreadRunnable::CoreThreadRunnable(Queue<Runnable *> &runnables, MutexLock &lock)
        : runnables(runnables), lock(lock) {}

[[noreturn]] void Executors::FixedThreadPool::CoreThreadRunnable::run() {
    while (true) {
        lock.lock();
        while (runnables.isEmpty()) {
            lock.wait();
        }
        Runnable *runnable = runnables.dequeue();
        lock.unlock();
        runnable->run();
    }
}

Executors::FixedThreadPool::FixedThreadPool(int poolSize) {
    this->poolSize = poolSize;
    coreThreads = new Thread *[poolSize];
    coreThreadRunnable = new CoreThreadRunnable(runnables, lock);
    for (int i = 0; i < poolSize; ++i) {
        coreThreads[i] = new Thread(coreThreadRunnable);
    }
}

Executors::FixedThreadPool::~FixedThreadPool() {
    for (int i = 0; i < poolSize; ++i) {
        delete coreThreads[i];
    }
    delete coreThreads;
    delete coreThreadRunnable;
}

void Executors::FixedThreadPool::execute(Runnable *runnable) {
    lock.lock();
    runnables.enqueue(runnable);
    lock.notify();
    lock.unlock();
}

ThreadPool *Executors::newFixedThreadPool(int poolSize) {
    return new FixedThreadPool(poolSize);
}

void *bczhc::concurrent::call(void *arg) {
    auto *runnable = (Runnable *) arg;
    runnable->run();
    return nullptr;
}
