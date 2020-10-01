#include "Sqlite3.h"

void bczhc::Sqlite3::close() {
    if (!closed) sqlite3_close(db);
}

void bczhc::Sqlite3::exec(const char *cmd, bczhc::Sqlite3::SqliteCallback &callback) {
    exec(cmd, callback, nullptr);
}

void bczhc::Sqlite3::exec(const char *cmd, bczhc::Sqlite3::SqliteCallback &callback, void *arg) {
    Bean bean(callback, arg);
    sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
}

bczhc::Sqlite3::~Sqlite3() {
    close();
    sqlite3_db_release_memory(db);
}

void bczhc::Sqlite3::open(const char *path) {
    int status = sqlite3_open(path, &db);
    if (status != SQLITE_OK) throw status;
}
