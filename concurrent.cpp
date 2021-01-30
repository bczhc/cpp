#include "concurrent.h"
#include <csignal>
#include <pthread.h>

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

void CountDownLatch::increaseCount() {
    lock.lock();
    ++count;
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

void LongWaitCountDownLatch::increaseCount() {
    lock.lock();
    ++count;
    lock.unlock();
}

void LongWaitCountDownLatch::wait() {
    if (count == 0) return;
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

void Thread::detach() const {
    pthread_detach(t);
}

void Thread::sleep(int64_t millis) {
    usleep(millis * 1000);
}

void Thread::terminate() const {
#ifdef SIGKILL
    sendSignal(SIGKILL);
#elif SIGTERM
    sendSignal(SIGTERM);
#endif
}

void Thread::sendSignal(int signal) const {
    pthread_kill(t, signal);
}
pthread_t Thread::getPThread() const {
    return this->t;
}

Executors::FixedThreadPool::CoreThreadRunnable::CoreThreadRunnable(Queue<Runnable *> &runnables, MutexLock &lock, bool &interrupted, LongWaitCountDownLatch *&waitAllLatch)
    : runnables(runnables), lock(lock), interrupted(interrupted), waitAllLatch(waitAllLatch) {}

void Executors::FixedThreadPool::CoreThreadRunnable::run() {
    while (true) {
        lock.lock();
        while (runnables.isEmpty()) {
            if (interrupted) {
                lock.unlock();
                //exit the current thread
                return;
            }
            lock.wait();
        }
        Runnable *runnable = runnables.dequeue();
        lock.unlock();
        runnable->run();
        waitAllLatch->countDown();
    }
}

Executors::FixedThreadPool::FixedThreadPool(int poolSize) {
    this->poolSize = poolSize;
    coreThreads = new Thread *[poolSize];
    waitAllLatch = new LongWaitCountDownLatch(0);
    coreThreadRunnable = new CoreThreadRunnable(runnables, lock, interrupted, waitAllLatch);
    for (int i = 0; i < poolSize; ++i) {
        coreThreads[i] = new Thread(coreThreadRunnable);
    }
}

Executors::FixedThreadPool::~FixedThreadPool() {
    if (!interrupted) terminateCoreThreads();
    for (int i = 0; i < poolSize; ++i) {
        coreThreads[i]->join();
        delete coreThreads[i];
    }
    delete waitAllLatch;
    delete[] coreThreads;
    delete coreThreadRunnable;
}

void Executors::FixedThreadPool::execute(Runnable *runnable) {
    lock.lock();
    runnables.enqueue(runnable);
    waitAllLatch->increaseCount();
    lock.notify();
    lock.unlock();
}

void Executors::FixedThreadPool::shutdown() {
    terminateCoreThreads();
}

void Executors::FixedThreadPool::waitAll() {
    waitAllLatch->wait();
}

void Executors::FixedThreadPool::terminateCoreThreads() {
    interrupted = true;
    for (int i = 0; i < poolSize; ++i) {
        lock.lock();
        lock.notify();
        lock.unlock();
    }
}

ThreadPool *Executors::newFixedThreadPool(int poolSize) {
    return new FixedThreadPool(poolSize);
}

void *bczhc::concurrent::call(void *arg) {
    auto *runnable = (Runnable *) arg;
    runnable->run();
    return nullptr;
}

void Latch::wait() {
    lock.lock();
    if (latched) lock.wait();
    lock.unlock();
}

void Latch::latch() {
    lock.lock();
    latched = true;
    lock.unlock();
}

void Latch::unlatch() {
    lock.lock();
    latched = false;
    lock.unlock();
}

void Latch::notify() {
    lock.lock();
    lock.notify();
    lock.unlock();
}

void Latch::unlatchAndNotify() {
    lock.lock();
    latched = false, lock.notify();
    lock.unlock();
}

Latch::Latch(bool latched) : latched(latched) {}

bool Latch::isLatched() const {
    return latched;
}
