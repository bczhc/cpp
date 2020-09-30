#include <iostream>
#include "../Concurrent.h"
#include "../zhc.h"

using namespace bczhc::concurrent;
using namespace std;

int main(int argc, char **argv) {
    class R : public Runnable {
    private:
        int i;
    public:
        void run() override {
            Thread::sleep(1000);
            cout << i << endl;
        }

        explicit R(int i) : i(i) {}
    };
    ThreadPool *pool = Executors::newFixedThreadPool(3);
    PointersSet set;
    for (int i = 0; i < 10; ++i) {
        Runnable *runnable = new R(i);
        set.put(runnable);
        pool->execute(runnable);
    }
    set.freeAll();
    delete pool;
    return 0;
}