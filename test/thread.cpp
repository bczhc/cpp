#include "../Concurrent.hpp"
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace bczhc;
using namespace concurrent;

void println(const char *s) { printf("%s\n", s); }

void sleep(int sec) { usleep(sec * 1000000); }

MutexLock lock;
using Condition = MutexLock::Condition;

int main() {
    ThreadPool *pool = Executors::newFixedThreadPool(2);
    class R : public Runnable {
    public:
        int a;
        void run() override { printf("%i\n", a); }
    };
    R *r[10];
    for (int i = 0; i < 10; ++i) {
        r[i] = new R();
        r[i]->a = i;
        pool->execute(*(r[i]));
    }
    for (int i = 0; i < 10; ++i) {
        delete r[i];
    }
    sleep(20);
    return 0;
}
