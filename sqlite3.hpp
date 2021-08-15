//
// Created by root on 2020/9/26.
//

#ifndef BCZHC_SQLITE3_HPP
#define BCZHC_SQLITE3_HPP

#include "third_party/sqlite3-single-c/sqlite3.h"
#include <cstdint>
#include "string.hpp"
#include "./exception.hpp"
#include "./def.hpp"

using namespace bczhc;

namespace bczhc {
    class Sqlite3 {
    public:
        class SqliteCallback {
        public:
            virtual int callback(void *arg, int colNum, char **content, char **colName) = 0;
        };

        class Statement;

        class Cursor {
        private:
            Statement &stmt;

        public:
            struct BlobResult {
                int size;
                const uchar *data;
            };

            explicit Cursor(Sqlite3::Statement &stmt);

            void reset() const;

            bool step() const;

            int32_t getInt(int column) const;

            int64_t getLong(int column) const;

            double getDouble(int column) const;

            const char *getText(int column) const;

            BlobResult getBlob(int column) const;
        };

        class Statement {
        public:
            sqlite3_stmt *stmt;
            sqlite3 *db;

            void step() const;

            Statement(sqlite3_stmt *stmt);

            Statement(const Sqlite3::Statement &stat);

            void reset() const;

            void bind(int row, int32_t a) const;

            void bind(int row, int64_t a) const;

            void bind(int row, double a) const;

            void bindText(int row, const char *s, void(*destructCallback)(void *) = SQLITE_STATIC) const;

            void bindText(int row, const char *s, int size, void(*destructCallback)(void *) = SQLITE_STATIC) const;

            void bindBlob(int row, const char *bytes, int size, void(*destructCallback)(void *) = SQLITE_STATIC) const;

            void bindNull(int row) const;

            void release() const;

            int stepRow() const;

            void clearBinding() const;

            Cursor getCursor() {
                Cursor c(*this);
                return c;
            }

            /**
             *
             * @param c column name
             * @return index, the leftmost value is 0
             */
            int getIndexByColumnName(const char *c) {
                int size = sqlite3_column_count(this->stmt);
                for (int i = 0; i < size; ++i) {
                    if (String::equal(c, sqlite3_column_name(this->stmt, i))) return i;
                }
                throw SqliteException("not found column by name", -1);
            }
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

        static String makeExceptionString(sqlite3 *db, const String &mainStatement, int errorCode);

        SqliteException makeException(const String &mainStatement, int errorCode) const;

        static SqliteException makeException(sqlite3 *db, const String &mainStatement, int errorCode);

    public:
        sqlite3 *db = nullptr;

        Sqlite3(const char *path);

        void close();

        void exec(const char *cmd, SqliteCallback &callback) const;

        void exec(const char *cmd) const;

        void exec(const char *cmd, SqliteCallback &callback, void *arg) const;

        bool checkIfCorrupt();

        Statement compileStatement(const char *sql) const;

        Sqlite3 &operator=(const Sqlite3 &a);

        Sqlite3(const Sqlite3 &a);

        const char *getErrorMsg() const;
    };
}

#endif //BCZHC_SQLITE3_HPP
