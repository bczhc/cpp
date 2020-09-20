#include "../Thread.hpp"
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace bczhc;
using namespace concurrent;

void println(const char *s) { printf("%s\n", s); }

void sleep(int sec) { usleep(sec * 1000000); }

MutexLock lock;
using Condition = MutexLock::Condition;
Condition cond1 = lock.newCondition();
Condition cond2 = lock.newCondition();

class R : public Runnale {
public:
    void run() override {
        lock.lock();
        println("task1 waiting...");
        cond1.wait();
        println("task1 running...");
        lock.unlock();
    }
} r1;

class R2 : public Runnale {
public:
    void run() override {
        lock.lock();
        println("task2 waiting...");
        cond2.wait();
        println("task2 running...");
        lock.unlock();
    }
} r2;
int main() {
    Thread t1(r1);
    Thread t2(r2);
    sleep(1), cond1.signal();
    sleep(1), cond2.signal();
    t1.join();
    t2.join();
    return 0;
}
