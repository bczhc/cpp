#include "../Concurrent.h"
#include <csignal>
#include <cstdio>

using namespace bczhc;
using namespace concurrent;

int main() {
    class R : public Runnable {
    private:
        static void sigHandler(int signal) {
            if (signal == SIGTERM) {
                printf("killed...\n");
                pthread_exit(PTHREAD_CANCELED);
            }
        }

    public:
        void run() override {
            signal(SIGTERM, sigHandler);
            Thread::sleep(1000);
            printf("Frailty, thy name is woman!\n");
        }
    };

    auto *r = new R;
    Thread t(r);
    Thread::sleep(500);
    //    t.terminate();
    delete r;
    t.sendSignal(SIGTERM);
    t.join();
    return 0;
}
