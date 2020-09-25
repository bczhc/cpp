#include "../Concurrent.hpp"
#include <cstdio>

using namespace concurrent;

CountDownLatch latch(10);

int main() {
    class R : public Runnable {
    private:
        int i = 0;

    public:
        void setI(int a) { i = a; }
        void run() override {
            printf("%i\n", i);
            latch.countDown();
        }
    };

    for (int i = 0; i < 10; ++i) {
        R *r = new R;
        r->setI(i);
        Thread t(r);
    }
    latch.await();
    return 0;
}
