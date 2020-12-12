#ifndef BCZHC_CONCURRENT_H
#define BCZHC_CONCURRENT_H

#include "./third_party/practice/LinearList.hpp"
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <cstdint>

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
                Condition(MutexLock &mutexLock) : mutex(mutexLock.mutexLock){};

                ~Condition();

                void wait();

                void signal();
            };

            pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;

            ~MutexLock();

            void lock();

            void unlock();

            bool tryLock();

            Condition newCondition();

        private:
            Condition mCond = newCondition();

        public:
            void wait();

            void notify();
        };

        class CountDownLatch {
        private:
            int count{};
            MutexLock lock;

        public:
            CountDownLatch(int count) : count(count) {}

            CountDownLatch();

            void await() const;

            void countDown();

            void set(int _count);
        };

        class LongWaitCountDownLatch {
        private:
            int count;
            MutexLock lock;

        public:
            LongWaitCountDownLatch(int count);

            void countDown();

            void wait();

            void interruptAndReset();
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

        void *call(void *arg);

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

        public:
            Thread(Runnable *runnable);

            void join() const;

            static void sleep(int64_t millis);

            void terminate();
        };

        class ThreadPool {
        public:
            virtual void execute(Runnable *r) = 0;

            virtual void shutdown() = 0;

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
                    CountDownLatch *terminateLatch;

                public:
                    CoreThreadRunnable(Queue<Runnable *> &runnables, MutexLock &lock, CountDownLatch *&terminateLatch);

                    void run() override;
                };

                MutexLock lock;
                Queue<Runnable *> runnables;
                int poolSize;
                Thread **coreThreads;
                CoreThreadRunnable *coreThreadRunnable;
                CountDownLatch *terminateLatch;

            public:
                FixedThreadPool(int poolSize);

                ~FixedThreadPool() override;

                void execute(Runnable *runnable) override;

                void shutdown() override;

                void terminateCoreThreads();
            };

            static ThreadPool *newFixedThreadPool(int poolSize);
        };
    }// namespace concurrent
}// namespace bczhc

#endif// BCZHC_CONCURRENT_H
