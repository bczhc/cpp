//
// Created by root on 2020/9/26.
//

#ifndef BCZHC_SQLITE3_HPP
#define BCZHC_SQLITE3_HPP

#include "third_party/sqlite3-single-c/sqlite3.h"
#include <cstdint>
#include "string.hpp"
#include "./exception.hpp"

using namespace bczhc;

namespace bczhc {
    class Sqlite3 {
    public:
        class SqliteCallback {
        public:
            virtual int callback(void *arg, int colNum, char **content, char **colName) = 0;
        };

        class Statement {
        public:
            sqlite3_stmt *stmt;

            void step() const;

            Statement(sqlite3_stmt *stmt);

            Statement(const Sqlite3::Statement &stat);

            void reset() const;

            void bind(int row, int32_t a) const;

            void bind(int row, int64_t a) const;

            void bind(int row, double a) const;

            void bindText(int row, const char *s) const;

            void bindText(int row, const char *s, int size) const;

            void bindBlob(int row, const char *bytes, int size) const;

            void bindNull(int row) const;

            void release() const;
        };

    private:
        class Bean {
        public:
            SqliteCallback &callback;
            void *&arg;

            Bean(SqliteCallback &callback, void *&arg) : callback(callback), arg(arg) {
            }
        };

        static int callbackFunction(void *arg, int colNum, char **content, char **colName) {
            Bean &bean = *(Bean *) arg;
            return bean.callback.callback(bean.arg, colNum, content, colName);
        }

        bool closed = false;
    public:
        sqlite3 *db = nullptr;
        char *errMsg = nullptr;

        Sqlite3(const char *path);

        void close();

        void exec(const char *cmd, SqliteCallback &callback);

        void exec(const char *cmd);

        void exec(const char *cmd, SqliteCallback &callback, void *arg);

        bool checkIfCorrupt();

        ~Sqlite3();

        Statement compileStatement(const char *sql) const;
    };
}

#endif //BCZHC_SQLITE3_HPP
