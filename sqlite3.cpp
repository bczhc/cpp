#include <cstdint>
#include "sqlite3.hpp"
#include "string.hpp"

using namespace bczhc;

void Sqlite3::close() {
    closed = true;
    int status = sqlite3_close(db);
    if (status) throw SqliteException("closing failed", status);
}

void Sqlite3::exec(const char *cmd, Sqlite3::SqliteCallback &callback) {
    exec(cmd, callback, nullptr);
}

void Sqlite3::exec(const char *cmd) {
    int r = sqlite3_exec(db, cmd, nullptr, nullptr, &errMsg);
    if (r) throw SqliteException("executing failed", r);
}

void Sqlite3::exec(const char *cmd, Sqlite3::SqliteCallback &callback, void *arg) {
    Bean bean(callback, arg);
    int status = sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
    if (status) throw SqliteException("executing failed", status);
}

Sqlite3::~Sqlite3() {
    if (!closed && db != nullptr) {
        close();
        closed = true;
    }
}

Sqlite3::Sqlite3(const char *path) {
    int status = sqlite3_open(path, &this->db);
    if (status != SQLITE_OK) {
        throw SqliteException("open failed", status);
    }
}

Sqlite3::Statement Sqlite3::compileStatement(const char *sql) const {
    sqlite3_stmt *stmt = nullptr;
    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (status) throw SqliteException("compilation failed", status);
    Statement r(stmt);
    return r;
}

bool Sqlite3::checkIfCorrupt() {
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

void Sqlite3::Statement::step() const {
    int r = sqlite3_step(this->stmt);
    if (r != SQLITE_DONE) throw SqliteException("stepping failed", r);
}

Sqlite3::Statement::Statement(sqlite3_stmt *stmt) : stmt(stmt) {}

void Sqlite3::Statement::bind(int row, int32_t i) const {
    int r = sqlite3_bind_int(stmt, row, i);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::reset() const {
    int r = sqlite3_reset(stmt);
    if (r) throw SqliteException("resetting failed", r);
}

void Sqlite3::Statement::bind(int row, int64_t a) const {
    int r = sqlite3_bind_int64(stmt, row, (sqlite3_int64) a);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::bind(int row, double a) const {
    int r = sqlite3_bind_double(stmt, row, a);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::bindNull(int row) const {
    int r = sqlite3_bind_null(stmt, row);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::bindText(int row, const char *s) const {
    bindText(row, s, -1);
}

void Sqlite3::Statement::bindBlob(int row, const char *bytes, int size) const {
    int r = sqlite3_bind_blob(stmt, row, bytes, size, SQLITE_STATIC);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::bindText(int row, const char *s, int size) const {
    int r = sqlite3_bind_text(stmt, row, s, size, SQLITE_STATIC);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::release() const {
    int r = sqlite3_finalize(this->stmt);
    if (r) throw SqliteException("releasing failed", r);
}

Sqlite3::Statement::Statement(const Sqlite3::Statement &stat) {
    this->stmt = stat.stmt;
}
