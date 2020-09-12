#include "../third_party/practice/LinearList.hpp"
#include "../zhc.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sqlite3.h>

using namespace bczhc::linearlist;
LinkedList<char *> list;

int callback(void *arg, int colNum, char **fields, char **cols) {
    int len = strlen(fields[0]);
    char *s = new char[len + 1];
    s[len] = '\0';
    strcpy(s, fields[0]);
    list.insert(s);
    return 0;
}

int main() {
    sqlite3 *db;
    sqlite3_open("/storage/emulated/0/diary.db", &db);
    sqlite3_exec(db, "select * from diary", callback, nullptr, nullptr);
    LinkedList<char *>::Iterator it = list.getIterator();
    if (it.moveToFirst()) {
        do {
            char *s = it.get();
            int dateI = 0;
            int len = strlen(s);
            char date[9];
            date[8] = '\0';
            int i = 0;
            for (; s[i] != '.'; ++i)
                date[dateI++] = s[i];
            if (s[i + 2] == '.') {
                date[dateI] = '0';
                date[dateI + 1] = s[i + 1];
            } else
                date[dateI] = s[i + 1], date[dateI + 1] = s[i + 2];
            dateI += 2;
            for (++i; s[i] != '.'; ++i) {
            }
            if (s[i + 2] == '\0') {
                date[dateI] = '0';
                date[dateI + 1] = s[i + 1];
            } else {
                date[dateI] = s[i + 1];
                date[dateI + 1] = s[i + 2];
            }
            dateI += 2;
            char *sql = nullptr;
            bczhc::strcpyAndCat_auto(&sql, "update diary set date='", -1, "", 0,
                                     false);
            bczhc::strcpyAndCat_auto(&sql, sql, -1, date, -1, true);
            bczhc::strcpyAndCat_auto(&sql, sql, -1, "' where date='", -1, true);
            bczhc::strcpyAndCat_auto(&sql, sql, -1, s, -1, true);
            bczhc::strcpyAndCat_auto(&sql, sql, -1, "'", -1, true);
            sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
            printf("%s\n", sql);
        } while (it.next());
    }
    sqlite3_close(db);
    return 0;
}
