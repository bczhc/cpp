#include "../concurrent.hpp"
#include "../file.hpp"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.hpp"
#include "../array.hpp"
#include "../def.hpp"
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
#include "../regex.hpp"
#include "../io.hpp"
#include <random>

using namespace std;
using namespace bczhc;

int main() {
    class R : public Runnable {
    public:
        void run() override {
            cout << "h" << endl;
            delete this;
        }
    };
    ThreadPool *pool = Executors::newFixedThreadPool(1);
    pool->execute(new R);
    pool->waitAll();
    pool->shutdown();
    delete pool;
    return 0;
}