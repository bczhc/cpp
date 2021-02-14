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

using namespace std;
using namespace bczhc;

int main() {
    uint32_t c1 = surrogateConvert(0xd83c, 0xdde6);
    uint32_t c2 = surrogateConvert(0xd83c, 0xddee);
    String s;
    s.appendUnicode(c1).appendUnicode(c2);
    cout << s.getCString() << endl;
    cout << s.utf8Length() << endl;
    cout << s.length() << endl;
    return 0;
}