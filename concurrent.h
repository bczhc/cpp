#ifndef BCZHC_CONCURRENT_H
#define BCZHC_CONCURRENT_H

#include "./third_party/practice/LinearList.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED ((void *) -1)
#endif

using namespace bczhc;

namespace bczhc {
    class MutexLock {
    public:
        class Condition {
        private:
            pthread_cond_t cond{};
            pthread_mutex_t *mutex = nullptr;

        public:
            Condition();

            explicit Condition(MutexLock &mutexLock);;

            void wait();

            void signal();

            void release();

            MutexLock::Condition &operator=(const MutexLock::Condition &a);

            Condition(const MutexLock::Condition &a);
        };

        pthread_mutex_t mutexLock{};
        Condition bundledCondition;

        MutexLock();

        void lock();

        void unlock();

        bool tryLock();

        void release();

        Condition newCondition();

        void wait();

        void notify();

        MutexLock &operator=(const MutexLock &a);

        MutexLock(const MutexLock &a);
    };

    class CountDownLatch {
    private:
        int count{};
        MutexLock lock;

    public:
        explicit CountDownLatch(int count);

        CountDownLatch();

        void await() const;

        void countDown();

        void increaseCount();

        void set(int _count);
    };

    class LongWaitCountDownLatch {
    private:
        int count;
        MutexLock lock;

    public:
        explicit LongWaitCountDownLatch(int count);

        void countDown();

        void increaseCount();

        void wait();

        void interruptAndReset();
    };

    class Latch {
    private:
        MutexLock lock;
        bool latched = false;
    public:
        explicit Latch(bool latched);

        void wait();

        void latch();

        void unlatch();

        void notify();

        void unlatchAndNotify();

        [[nodiscard]] bool isLatched() const;
    };

    class Runnable {
    public:
        virtual void run() = 0;
    };

    template<typename ArgType>
    class Consumer {
    public:
        virtual void accept(ArgType &arg) = 0;
    };

    template<typename T>
    struct Bean {
        Consumer<T> *c;
        void *arg;
    };

    template<typename ArgType>
    class ConsumerCall {
    public:
        static void consume(ArgType &arg) {}

        static void *call(void *arg) {
            Bean<ArgType> &b = *(Bean<ArgType> *) arg;
            ((Consumer<ArgType> *) b.c)->accept(*(ArgType *) b.arg);
            delete &b;
            return nullptr;
        }
    };

    class Thread {
    private:
        pthread_t t{};

        static void *call(void *arg);

    public:
        explicit Thread(Runnable *runnable);

        void join() const;

        void detach() const;

        static void sleep(int64_t millis);

        void terminate() const;

        void sendSignal(int signal) const;

        [[nodiscard]] pthread_t getPThread() const;

        Thread &operator=(const Thread &a);

        Thread(const Thread &a);
    };

    class ThreadPool {
    public:
        virtual void execute(Runnable *r) = 0;

        virtual void shutdown() = 0;

        virtual void waitAll() = 0;

        virtual ~ThreadPool() = default;
    };

    class Executors {
    public:
        class FixedThreadPool : public ThreadPool {
        private:
            class CoreThreadRunnable : public Runnable {
            private:
                Queue<Runnable *> &runnables;
                MutexLock &lock;
                bool &interrupted;
                bczhc::LongWaitCountDownLatch *&waitAllLatch;

            public:
                CoreThreadRunnable(Queue<Runnable *> &runnables, MutexLock &lock, bool &interrupted,
                                   bczhc::LongWaitCountDownLatch *&waitAllLatch);

                void run() override;
            };

            MutexLock lock;
            Queue<Runnable *> runnables;
            int poolSize;
            Thread **coreThreads;
            CoreThreadRunnable *coreThreadRunnable;
            bool interrupted = false;
            bczhc::LongWaitCountDownLatch *waitAllLatch;

        public:
            explicit FixedThreadPool(int poolSize);

            ~FixedThreadPool() override;

            void execute(Runnable *runnable) override;

            void shutdown() override;

            void waitAll() override;

            void terminateCoreThreads();
        };

        static ThreadPool *newFixedThreadPool(int poolSize);
    };
}// namespace bczhc

#endif// BCZHC_CONCURRENT_H
