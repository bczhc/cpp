#include "../concurrent.h"
#include "../file.h"
#include "../io.h"
#include "../math/fourier_series.h"
#include "../regex.h"
#include "../sqlite3.h"
#include "../string.hpp"
#include "../third_party/practice/LinearList.hpp"
#include "../third_party/practice/SymbolTable.hpp"
#include "../utils.hpp"
#include "thread"
#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <ifaddrs.h>
#include <iostream>
#include <netinet/in.h>
#include <new>
#include <pcre.h>
#include <pthread.h>
#include <string>
#include "../array.hpp"
#include <fcntl.h>
#include <cstdarg>
#include <cassert>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;
using namespace bczhc::array;
using namespace symboltable;

using uchar = uint8_t;

template<typename T>
using Tuple = Array<T>;

template<typename ArgType, typename ReturnType>
Tuple<ReturnType> f(int size, ArgType a...) {
    Tuple<ReturnType> r(size);
    r[0] = a;
    va_list args{};
    va_start(args, a);
    for (int i = 1; i < size; ++i) {
        r[i] = va_arg(args, ArgType);
    }
    va_end(args);
    return r;
}

class Generator {
public:
    int r{};
    Latch continueLatch = Latch(true);
    Latch returnLatch = Latch(true);
    Thread *t = nullptr;
    Latch startedLatch = Latch(true);
    bool threadEnd = false;
    Latch receivedReturnNotifyLatch = Latch(true);

    void startThread() {
        class R : public Runnable {
        public:
            Generator *g;

            void run() override {
                g->startedLatch.unlatchAndNotify();
                for (int i = 0; i < 10; ++i) {
                    //cout << "yield " << i << endl;
                    g->yield(i);
                }
                g->returnLatch.unlatchAndNotify();
                g->threadEnd = true;
                delete this;
            }

            explicit R(Generator *g) : g(g) {}
        };

        auto *run = new R(this);
        t = new Thread(run);
    }

    int next() {
        //cout << "next" << endl;
        returnLatch.latch();
        if (t == nullptr) {
            //cout << "start thread" << endl;
            startThread();
        }
        //cout << "notify continueLatch" << endl;
        continueLatch.unlatchAndNotify();
        //cout << "wait for result" << endl;
        returnLatch.wait();
        receivedReturnNotifyLatch.unlatchAndNotify();
        if (this->threadEnd) throw String("no more yield");
        //cout << "return result" << endl;
        return this->r;
    }

    void yield(int i) {
        //cout << "set result" << endl;
        this->r = i;
        //cout << "notify returnLatch" << endl;
        receivedReturnNotifyLatch.latch();
        returnLatch.unlatchAndNotify();
        receivedReturnNotifyLatch.wait();
        //cout << "wait for continue" << endl;
        continueLatch.wait();
        //cout << "continue" << endl;
        continueLatch.latch();
    }

    ~Generator() {

    }
};


int main(int argc, char **argv) {
    return 0;
}
