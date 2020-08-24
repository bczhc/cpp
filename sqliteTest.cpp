#include <sqlite3.h>
#include <iostream>
#include "./zhc.h"

using namespace std;

int callback(void *data, int argc, char **argv, char **azColName) {
    int *a = (int *) data;
    ::cout << (*a)++ << ' ' << argv[0] << ' ' << argv[1] << ' ' << argv[2] << ::endl;
    return 0;
}

int main(int argc, char **argv) {
    char a[] = "/storage/emulated/0/doc.db";
    if (argc <= 1) argv[1] = a;
    sqlite3 *db;
    char *erroMsg = nullptr;
    int status = sqlite3_open(argv[1], &db);
    if (status) {
        ::cout << "opening database failed" << ::endl;
    } else ::cout << "opening database succeeded" << ::endl;

    char sql[] = "SELECT * FROM doc";
    int i = 0;
    // status = sqlite3_exec(db, sql, callback, &i, &erroMsg);
    if (status) sqlite3_errmsg(db), exit(1);
    sqlite3_close(db);
    return 0;
}
