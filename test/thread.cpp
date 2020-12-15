#include "../Concurrent.h"
#include <cstdio>
#include <pthread.h>

using namespace bczhc;
using namespace concurrent;

int main() {
    class R : public Runnable {
    public:
        void run() override {
            Thread::sleep(1000);
            printf("脆弱啊，你的名字就是女人。\n");
            delete this;
        }
    };

    ThreadPool *pool = Executors::newFixedThreadPool(4);
    for (int i = 0; i < 10; ++i) {
        pool->execute(new R);
    }
    pool->waitAll();
    delete pool;
    return 0;
}
