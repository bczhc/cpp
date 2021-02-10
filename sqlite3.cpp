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

void Sqlite3::exec(const char *cmd) const {
    char *errMsg = nullptr;
    int r = sqlite3_exec(db, cmd, nullptr, nullptr, &errMsg);
    if (r) {
        String msg = "executing failed, msg: ";
        msg += errMsg;
        throw SqliteException(msg.getCString(), r);
    }
}

void Sqlite3::exec(const char *cmd, Sqlite3::SqliteCallback &callback, void *arg) const {
    char *errMsg = nullptr;
    Bean bean(callback, arg);
    int status = sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
    if (status) {
        String msg = "executing failed, msg: ";
        msg += errMsg;
        throw SqliteException(msg.getCString(), status);
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

Sqlite3 &Sqlite3::operator=(const Sqlite3 &a) {
    if (&a == this) return *this;
    this->db = a.db;
    this->closed = a.closed;
    return *this;
}

Sqlite3::Sqlite3(const Sqlite3 &a) {
    operator=(a);
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

void Sqlite3::Statement::release() const {
    int r = sqlite3_finalize(this->stmt);
    if (r) throw SqliteException("releasing failed", r);
}

Sqlite3::Statement::Statement(const Sqlite3::Statement &stat) {
    this->stmt = stat.stmt;
}

int Sqlite3::Statement::stepRow() const {
    return sqlite3_step(this->stmt);
}

void Sqlite3::Statement::clearBinding() const {
    int status = sqlite3_clear_bindings(this->stmt);
    if (status != SQLITE_OK) throw SqliteException("clearing failed", status);
}

void Sqlite3::Statement::bindText(int row, const char *s, void (*destructCallback)(void *)) const {
    bindText(row, s, -1, destructCallback);
}

void Sqlite3::Statement::bindText(int row, const char *s, int size, void (*destructCallback)(void *)) const {
    int r = sqlite3_bind_text(stmt, row, s, size, destructCallback);
    if (r) throw SqliteException("binding failed", r);
}

void Sqlite3::Statement::bindBlob(int row, const char *bytes, int size, void (*destructCallback)(void *)) const {
    int r = sqlite3_bind_blob(stmt, row, bytes, size, destructCallback);
    if (r) throw SqliteException("binding failed", r);
}

bool Sqlite3::Cursor::step() const {
    return stmt.stepRow() == SQLITE_ROW;
}

void Sqlite3::Cursor::reset() const {
    stmt.reset();
}

Sqlite3::Cursor::Cursor(Sqlite3::Statement &stmt) : stmt(stmt) {
}

const uchar *Sqlite3::Cursor::getBlob(int column) const {
    return (const uchar *) sqlite3_column_blob(stmt.stmt, column);
}

const char *Sqlite3::Cursor::getText(int column) const {
    return (const char *) sqlite3_column_text(stmt.stmt, column);
}

double Sqlite3::Cursor::getDouble(int column) const {
    return sqlite3_column_double(stmt.stmt, column);
}

int64_t Sqlite3::Cursor::getLong(int column) const {
    return sqlite3_column_int64(stmt.stmt, column);
}

int32_t Sqlite3::Cursor::getInt(int column) const {
    return sqlite3_column_int(stmt.stmt, column);
}
