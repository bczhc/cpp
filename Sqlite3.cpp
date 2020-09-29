//
// Created by root on 2020/9/26.
//

#include <cstdio>
#include "Sqlite3.h"

#ifndef BCZHC_SQLITE3_HPP
#define BCZHC_SQLITE3_HPP

#include "third_party/sqlite3-single-c/sqlite3.h"

namespace bczhc {
    class Sqlite3 {
    public:
        class SqliteCallback {
        public:
            virtual int callback(void *arg, int colNum, char **content, char **colName) = 0;
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

        sqlite3 *db = nullptr;
        char *errMsg = nullptr;
        bool closed = false;
    public:
        Sqlite3(const char *path) {
            int status = sqlite3_open(path, &db);
            if (status != SQLITE_OK) throw status;
        }

        void close() {
            if (!closed) sqlite3_close(db);
        }

        inline void exec(const char *cmd, SqliteCallback &callback) {
            exec(cmd, callback, nullptr);
        }

        inline void exec(const char *cmd, SqliteCallback &callback, void *arg) {
            Bean bean(callback, arg);
            sqlite3_exec(db, cmd, callbackFunction, &bean, &errMsg);
        }

        ~Sqlite3() {
            close();
            sqlite3_db_release_memory(db);
        }
    };
}

#endif //BCZHC_SQLITE3_HPP
