#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include "../utils.h"
#include "../File.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;

MutexLock lock; // NOLINT(cert-err58-cpp)
CountDownLatch latch(2); // NOLINT(cert-err58-cpp)

int main() {
    ThreadPool *pool = Executors::newFixedThreadPool(4);
    class R : public Runnable {
    public:
        void run() override {
            cout << 1 << endl;
            latch.countDown();
            delete this;
        }
    };
    pool->execute(new R());
    pool->execute(new R());
    latch.await();
    pool->shutdown();
    delete pool;
    return 0;
}
