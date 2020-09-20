#include "./third_party/practice/LinearList.hpp"
#include <cstdio>
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
};

class Runnale {
public:
    virtual void run() = 0;
};

template <typename ArgType> class Consumer {
public:
    virtual void accept(ArgType &arg) = 0;
};

void *call(void *arg) {
    ((Runnale *)arg)->run();
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
    Thread(Runnale &runnable) { pthread_create(&t, nullptr, call, &runnable); }

    template <typename ArgType>
    explicit Thread(Consumer<ArgType> &consumer, ArgType &arg) {
        Bean<ArgType> *b = new Bean<ArgType>;
        b->arg = &arg, b->c = &consumer;
        pthread_create(&t, nullptr, ConsumerCall<ArgType>::call, b);
    }

    void join() { pthread_join(t, nullptr); }
};

class ThreadPool {
public:
    virtual void execute(Runnale &r) = 0;
};

class Executors {
public:
    class FixedThreadPool : public ThreadPool {
    private:
        SequentialList<Runnale *> runnables;
        Thread **coreThreads;
        Thread *pullTasksThread;

    public:
        FixedThreadPool(int poolSize) { coreThreads = new Thread *[poolSize]; }

        void execute(Runnale &runnable) override {}
    };
    ThreadPool *newFixedThreadPool(int poolSize) {
        FixedThreadPool *pool = new FixedThreadPool(poolSize);
        return pool;
    }
};
} // namespace concurrent
} // namespace bczhc
