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

extern "C" {
#include "../third_party/crypto-algorithms/sha256.h"
}

using namespace std;
using namespace bczhc;

int main() {
    auto *db = new Sqlite3("");
    db->exec("CREATE TABLE IF NOT EXISTS a\n(\n    a\n)");
    db->exec("INSERT INTO a VALUES (1)");
    db->close();
    delete db;
    return 0;
}