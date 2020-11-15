#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include <sys/time.h>


using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

int main() {
    Sqlite3 db;
    db.open("./test.db");
    db.exec("BEGIN TRANSACTION");
    db.exec("DROP TABLE IF EXISTS a");
    db.exec("CREATE TABLE IF NOT EXISTS a(v1,v2)");

    const Sqlite3::Statement statement = db.compileStatement("INSERT INTO a VALUES(?,?)");
    if (statement.status != SQLITE_OK) return 1;
    int c = 0;
    timeval start{}, t{};
    gettimeofday(&start, nullptr);
    for (;;) {
        statement.reset();
        statement.bind(1, c);
        statement.bind(2, "hello, world");
        if (statement.step() != SQLITE_DONE) return 1;
        ++c;
        gettimeofday(&t, nullptr);
        if (t.tv_sec * 1000 + t.tv_usec / 1000 - start.tv_sec * 1000 - start.tv_usec / 1000 >= 1000) break;
    }
    db.exec("COMMIT");
    db.close();
    cout << c << endl;
    return 0;
}
