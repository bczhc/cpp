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
    String s;
    auto start = getCurrentTimeMillis();
    for (int i = 0; i < 10000000; ++i) {
        s.append("a");
    }
    auto end = getCurrentTimeMillis();
    cout << (end - start) << endl;
    return 0;
}