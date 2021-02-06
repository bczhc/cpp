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
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#ifdef __WIN32
#include <windows.h>
#endif
#ifdef CMAKE_PCRE_FOUND
#include <pcre.h>
#endif
#include "../array.hpp"
#include <cassert>
#include <cstdarg>
#include <fcntl.h>
#include <pthread.h>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace bczhc::array;
using namespace symboltable;

int main() {
    Sqlite3 db;
    db.open("/home/zhc/code/cpp/test/a.db");
    db.exec("CREATE TABLE IF NOT EXISTS a\n(\n    a,\n    b,\n    c\n)");
    Sqlite3::Statement statement = db.compileStatement("INSERT INTO a\nVALUES (?,?,?)");
    for (int i = 0; i < 50; ++i) {
        statement.reset();
         String string1 = String::toString(i);
        statement.bindText(1, string1.getCString());
        statement.bind(2, i);
        statement.bindNull(3);
        statement.step();
    }
    return 0;
}
