#include "../Thread.hpp"
#include <cstdio>
#include <thread>
#include <unistd.h>

using namespace bczhc;
using namespace thread;

int main() {
    class R : public Runnale {
        void run() override {
            usleep(1000000);
            printf("a\n");
        }
    } r;
    class ConsumerImpl : public Consumer<int> {
        void accept(int &arg) override {
            usleep(1000000);
            printf("%i\n", arg);
        }
    } c;
    return 0;
}
