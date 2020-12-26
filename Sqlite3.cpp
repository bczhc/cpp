#include <cstdint>
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
    if (!closed && db != nullptr) {
        close();
    }
}

int bczhc::Sqlite3::open(const char *path) {
    return sqlite3_open(path, &db);
}

bczhc::Sqlite3::Statement bczhc::Sqlite3::compileStatement(const char *sql) const {
    sqlite3_stmt *stmt = nullptr;
    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    Statement r(stmt, status);
    return r;
}

bool bczhc::Sqlite3::checkIfCorrupt() {
    bool r = true;
    class CB : public SqliteCallback {
    public:
        bool &r;

        explicit CB(bool &r) : r(r) {}

        int callback(void *arg, int colNum, char **content, char **colName) override {
            if (String::equal("ok", content[0])) r = false;
            return 0;
        }
    } cb(r);

    exec("PRAGMA integrity_check", cb);
    return r;
}

int bczhc::Sqlite3::Statement::step() const {
    return sqlite3_step(this->stmt);
}

bczhc::Sqlite3::Statement::Statement(sqlite3_stmt *stmt, int status) : stmt(stmt), status(status) {}

int bczhc::Sqlite3::Statement::bind(int row, int i) const {
    return sqlite3_bind_int(stmt, row, i);
}

int bczhc::Sqlite3::Statement::reset() const {
    return sqlite3_reset(stmt);
}

int bczhc::Sqlite3::Statement::bind(int row, int64_t a) const {
    return sqlite3_bind_int64(stmt, row, (sqlite3_int64) a);
}

int bczhc::Sqlite3::Statement::bind(int row, double a) const {
    return sqlite3_bind_double(stmt, row, a);
}

int bczhc::Sqlite3::Statement::bindNull(int row) const {
    return sqlite3_bind_null(stmt, row);
}

int bczhc::Sqlite3::Statement::bindText(int row, const char *s) const {
    return bindText(row, s, -1);
}

int bczhc::Sqlite3::Statement::bindBlob(int row, const char *bytes, int size) const {
    return sqlite3_bind_blob(stmt, row, bytes, size, SQLITE_STATIC);
}

int bczhc::Sqlite3::Statement::bindText(int row, const char *s, int size) const {
    return sqlite3_bind_text(stmt, row, s, size, SQLITE_STATIC);
}
