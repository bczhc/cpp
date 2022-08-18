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
#include <termios.h>
#include <cstdarg>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../app/charactersStatistics/CountCharacters.h"

using namespace std;
using namespace bczhc;

int main() {
    Sqlite3 db("db");
    db.key("1234");
    db.exec("create table if not exists a(a)");
    db.exec("insert into a values(123)");

    auto stmt = db.compileStatement("SELECT a FROM a");
    auto c = stmt.getCursor();
    while (c.step()) {
        auto i = c.getInt(0);
        cout << i << endl;
    }

    stmt.release();

    db.close();

    return 0;
}
