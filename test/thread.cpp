#include "../Concurrent.h"
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace bczhc;
using namespace concurrent;

void println(const char *s) { printf("%s\n", s); }

void sleep(int sec) { usleep(sec * 1000000); }

CountDownLatch latch(5);

int main() {
    class R : public Runnable {
    public:
        int i{};
        void run() override {
            sleep(3);
            printf("%i\n", i);
            latch.countDown();
            delete this;
        }
    };
    ThreadPool *pool = Executors::newFixedThreadPool(2);
    for (int i = 0; i < 5; ++i) {
        R *r = new R;
        r->i = i;
        pool->execute(r);
        sleep(1);
    }
    latch.await();
    delete pool;
    return 0;
}
