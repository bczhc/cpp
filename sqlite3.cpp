#include "sqlite3.hpp"
#include "string.hpp"
#include <cstdint>

using namespace bczhc;

String Sqlite3::makeExceptionString(sqlite3 *db, const String &mainStatement, int errorCode) {
    String msg = mainStatement + ", error code: ";
    msg.append(errorCode).append(", msg: ").append(sqlite3_errmsg(db));
    return msg;
}

SqliteException Sqlite3::makeException(sqlite3 *db, const String &mainStatement, int errorCode) {
    return {makeExceptionString(db, mainStatement, errorCode), errorCode};
}

SqliteException Sqlite3::makeException(const String &mainStatement, int errorCode) const {
    return {makeExceptionString(this->db, mainStatement, errorCode), errorCode};
}

void Sqlite3::close() {
    closed = true;
    int status = sqlite3_close(db);
    if (status) {
        throw makeException("closing failed", status);
    }
}

void Sqlite3::exec(const char *cmd, Sqlite3::SqliteCallback &callback) const {
    exec(cmd, callback, nullptr);
}

void Sqlite3::exec(const char *cmd) const {
    char *errMsg = nullptr;
    int r = sqlite3_exec(db, cmd, nullptr, nullptr, &errMsg);
    if (r) {
        throw makeException("execution failed", r);
    }
}

void Sqlite3::exec(const char *cmd, Sqlite3::SqliteCallback &callback, void *arg) const {
    char *errMsg = nullptr;
    Bean bean(callback, arg);
    int status = sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
    if (status) {
        throw makeException("execution failed", status);
    }
}

Sqlite3::Sqlite3(const char *path) {
    int status = sqlite3_open(path, &this->db);
    if (status != SQLITE_OK) {
        throw makeException("open failed", status);
    }
}

Sqlite3::Statement Sqlite3::compileStatement(const char *sql) const {
    sqlite3_stmt *stmt = nullptr;
    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (status) {
        throw makeException("compilation failed", status);
    }
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

    try {
        exec("PRAGMA integrity_check", cb);
    } catch (const SqliteException &e) {
        return true;
    }
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

const char *Sqlite3::getErrorMsg() const {
    return sqlite3_errmsg(this->db);
}

void Sqlite3::key(const String &key) const {
    int i = sqlite3_key(this->db, key.getCString(), (int) key.length());
    if (i != 0) {
        throw makeException(this->db, "sqlite3_key failed", i);
    }
}
void Sqlite3::rekey(const String &key) const {
    int r = sqlite3_rekey(this->db, key.getCString(), (int) key.length());
    if (r != 0) {
        throw makeException(this->db, "sqlite3_rekey failed", r);
    }
}

void Sqlite3::Statement::step() const {
    checkIfReleased();
    int r = sqlite3_step(this->stmt);
    if (r != SQLITE_DONE) {
        throw makeException(this->db, "stepping failed", r);
    }
}

Sqlite3::Statement::Statement(sqlite3_stmt *stmt) : stmt(stmt), db(sqlite3_db_handle(stmt)) {}

void Sqlite3::Statement::bind(int row, int32_t i) const {
    checkIfReleased();
    int r = sqlite3_bind_int(stmt, row, i);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

void Sqlite3::Statement::reset() const {
    checkIfReleased();
    int r = sqlite3_reset(stmt);
    if (r) {
        throw makeException(this->db, "reset failed", r);
    }
}

void Sqlite3::Statement::bind(int row, int64_t a) const {
    checkIfReleased();
    int r = sqlite3_bind_int64(stmt, row, (sqlite3_int64) a);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

void Sqlite3::Statement::bind(int row, double a) const {
    checkIfReleased();
    int r = sqlite3_bind_double(stmt, row, a);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

void Sqlite3::Statement::bindNull(int row) const {
    checkIfReleased();
    int r = sqlite3_bind_null(stmt, row);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

void Sqlite3::Statement::release() {
    checkIfReleased();
    int r = sqlite3_finalize(this->stmt);
    if (r) {
        throw makeException(this->db, "release failed", r);
    }
    this->released = true;
}

Sqlite3::Statement::Statement(const Sqlite3::Statement &stat) {
    this->stmt = stat.stmt;
    this->db = sqlite3_db_handle(stat.stmt);
}

bool Sqlite3::Statement::stepRow() const {
    checkIfReleased();
    int r = sqlite3_step(this->stmt);
    if (r == SQLITE_ROW) { return true; }
    if (r == SQLITE_DONE) { return false; }
    throw makeException(this->db, "step failed", r);
}

void Sqlite3::Statement::clearBinding() const {
    checkIfReleased();
    int status = sqlite3_clear_bindings(this->stmt);
    if (status != SQLITE_OK) {
        throw makeException(this->db, "clearing failed", status);
    }
}

void Sqlite3::Statement::bindText(int row, const char *s, void (*destructCallback)(void *)) const {
    bindText(row, s, -1, destructCallback);
}

void Sqlite3::Statement::bindText(int row, const char *s, int size, void (*destructCallback)(void *)) const {
    checkIfReleased();
    int r = sqlite3_bind_text(stmt, row, s, size, destructCallback);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

void Sqlite3::Statement::bindBlob(int row, const char *bytes, int size, void (*destructCallback)(void *)) const {
    checkIfReleased();
    int r = sqlite3_bind_blob(stmt, row, bytes, size, destructCallback);
    if (r) {
        throw makeException(this->db, "binding failed", r);
    }
}

int Sqlite3::Statement::getIndexByColumnName(const char *c) const {
    checkIfReleased();
    int size = sqlite3_column_count(this->stmt);
    for (int i = 0; i < size; ++i) {
        if (String::equal(c, sqlite3_column_name(this->stmt, i))) return i;
    }
    throw SqliteException("not found column by name", -1);
}

void Sqlite3::Statement::checkIfReleased() const {
    if (released) {
        throw SqliteException("Already released");
    }
}

Sqlite3::Cursor Sqlite3::Statement::getCursor() {
    checkIfReleased();
    Cursor c(*this);
    return c;
}

bool Sqlite3::Statement::isReleased() const {
    return this->released;
}

bool Sqlite3::Cursor::step() const {
    return stmt.stepRow();
}

void Sqlite3::Cursor::reset() const {
    stmt.reset();
}

Sqlite3::Cursor::Cursor(Sqlite3::Statement &stmt) : stmt(stmt) {
}

Sqlite3::Cursor::BlobResult Sqlite3::Cursor::getBlob(int column) const {
    int size = sqlite3_column_bytes(stmt.stmt, column);
    auto data = (const uchar *) sqlite3_column_blob(stmt.stmt, column);
    return BlobResult {
            .size = size,
            .data = data
    };
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
