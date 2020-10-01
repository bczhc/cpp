#include "Sqlite3.h"

int bczhc::Sqlite3::close() {
    closed = true;
    return sqlite3_close(db);
}

int bczhc::Sqlite3::exec(const char *cmd, bczhc::Sqlite3::SqliteCallback &callback) {
    return exec(cmd, callback, nullptr);
}

int bczhc::Sqlite3::exec(const char *cmd) {
    return sqlite3_exec(db, cmd, nullptr, nullptr, &errMsg);
}

int bczhc::Sqlite3::exec(const char *cmd, bczhc::Sqlite3::SqliteCallback &callback, void *arg) {
    Bean bean(callback, arg);
    return sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
}

bczhc::Sqlite3::~Sqlite3() {
    if (!closed) {
        close();
        sqlite3_db_release_memory(db);
    }
}

int bczhc::Sqlite3::open(const char *path) {
    return sqlite3_open(path, &db);
}
