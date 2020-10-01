//
// Created by root on 2020/9/26.
//

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
        void open(const char *path);

        void close();

        void exec(const char *cmd, SqliteCallback &callback);

        void exec(const char *cmd, SqliteCallback &callback, void *arg);

        ~Sqlite3();
    };
}

#endif //BCZHC_SQLITE3_HPP