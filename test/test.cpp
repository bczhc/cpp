#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include "../utils.h"
#include "../File.h"
#include "../io.h"

using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;

MutexLock lock; // NOLINT(cert-err58-cpp)

int main() {
    Sqlite3 db;
    db.open("./wubi_dict.db");
    db.exec("BEGIN TRANSACTION");
    db.exec("DROP TABLE IF EXISTS wubi_dict");
    db.exec("CREATE TABLE wubi_dict (\n    code,\n    char\n)");
    InputStream is("/home/zhc/rime-wubi86-jidian/wubi86_jidian.dict.yaml");
    String line;
    char c;
    String sql;
    String separator = "\t";
    while (is.read(&c, 1) > 0) {
        if (c == '\n') {
            SequentialList<String> split = line.split(separator);
            sql.clear();
            sql.append("insert into wubi_dict values('")
            .append(split.get(1))
            .append("','")
            .append(split.get(0))
            .append("')");

            const char *cmd = sql.getCString();
            printf("%s\n", cmd);
            db.exec(cmd);
            line.clear();
            continue;
        }
        line.append(c);
    }
    is.close();
    db.exec("COMMIT");
    db.close();
    return 0;
}
