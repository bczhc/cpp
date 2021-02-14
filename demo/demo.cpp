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
#include <memory>
#include "../array_list.hpp"
#include "../linked_list.hpp"
#include "../stack.hpp"
#include "../queue.hpp"
#include "../iterator.hpp"
#include "../symbol_table.hpp"
#include "../shared_pointer.hpp"
#include <unordered_map>
#include "stdatomic.h"

using namespace std;
using namespace bczhc;

static atomic_int i;
pthread_spinlock_t lock;

int main() {
    class R : public Runnable {
    public:
        void run() override {
            for (int j = 0; j < 10000; ++j) {
                pthread_spin_lock(&lock);
                ++i;
                pthread_spin_unlock(&lock);
            }
            delete this;
        }
    };

    pthread_spin_init(&lock, 0);


    Array<Thread> a(100);
    for (int j = 0; j < a.length(); ++j) {
        a[j] = Thread(new R);
    }

    for (int j = 0; j < a.length(); ++j) {
        a[j].join();
    }

    pthread_spin_destroy(&lock);
    cout << i << endl;
    return 0;
}