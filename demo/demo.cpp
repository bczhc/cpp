#include "../concurrent.h"
#include "../file.h"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.h"
#include "../array.hpp"
#include "../def.h"
#include <cassert>
#include "../app/base128/Base128Lib.h"
#include "../sqlite3.hpp"

using namespace std;
using namespace bczhc;

class Foo {
public:
    int k;
    int flag = 1;
    MutexLock lock;
    using Condition = MutexLock::Condition;
    Condition c1, c2;

    explicit Foo(int k) : k(k) {
        lock = MutexLock();
        c1 = lock.newCondition();
        c2 = lock.newCondition();
    }

    void p1() {
        for (int i = 0; i < k; ++i) {
            lock.lock();
            if (flag != 1) {
                c1.wait();
            }
            cout << "p1" << endl;
            cout.flush();
            flag = 2;
            c2.signal();
            lock.unlock();
        }
    }

    void p2() {
        for (int i = 0; i < k; ++i) {
            lock.lock();
            if (flag != 2) {
                c2.wait();
            }
            cout << "p2" << endl;
            cout.flush();
            flag = 1;
            c1.signal();
            lock.unlock();
        }
    }

    ~Foo() {
        c1.release();
        c2.release();
        lock.release();
    }
};

int main() {
    Foo foo(1000);
    class R : public Runnable {
    private:
        Foo &foo;
        int i;
    public:
        void run() override {
            if (i == 1) {
                foo.p1();
            } else {
                foo.p2();
            }
            delete this;
        }

        explicit R(Foo &foo, int i) : foo(foo), i(i) {}
    };
    R *r1 = new R(foo, 1);
    R *r2 = new R(foo, 2);

    Thread t1(r1);
    Thread t2(r2);

    t1.join(), t2.join();

    String s = "a";
    cout << ("a" == s) << endl;

    return 0;
}