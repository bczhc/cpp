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
#include "../doubly_linked_list.hpp"
#include "../queue.hpp"
#include "../stack.hpp"

using namespace std;
using namespace bczhc;

int main() {
    SymbolTable<String, String> a;
    for (int i = 0; i < 10; ++i) {
        a.put(String::toString(i), String::toString(i));
    }

    cout << a.get("a4").getCString() << endl;

    return 0;
}