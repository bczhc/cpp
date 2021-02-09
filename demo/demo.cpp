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

int main() {
    Sqlite3 db("/home/zhc/private-repo/diary.db");
    auto stmt = db.compileStatement("SELECT * FROM diary");
    auto dateIndex = stmt.getIndexByColumnName("date");
    auto contentIndex = stmt.getIndexByColumnName("content");
    cout << dateIndex << ' ' << contentIndex << endl;
    auto c = stmt.getCursor();
    while (c.step()) {
        int d = c.getInt(dateIndex);
        auto s = String(c.getText(contentIndex), 21);
        cout << d << ' ' << s.getCString() << endl;
    }

    stmt.release();
    db.close();
    return 0;
}
