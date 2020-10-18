#include "../Concurrent.h"
#include "../zhc.h"
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace bczhc;
using namespace concurrent;

void println(const char *s) { printf("%s\n", s); }

void sleep(int sec) { usleep(sec * 1000000); }

CountDownLatch latch(10);

int main() {
    class R : public Runnable {
    public:
        int i;
        void run() override {
            sleep(1);
            printf("%i\n", i);
            latch.countDown();
        }
    };
    ThreadPool *pool = Executors::newFixedThreadPool(2);
    PointersSet ps;
    for (int i = 0; i < 10; ++i) {
        R *r = new R;
        ps.put(r);
        r->i = i;
        pool->execute(r);
    }
    latch.await();
    free(pool);
    ps.freeAll();
    return 0;
}
