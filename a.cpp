#include <sqlite3.h>
#include <iostream>

using namespace std;

typedef int(*Callback)(void *arg, int colNum, char **content, char **colName);

int checkIfExistCallback(void *arg, int colNum, char **content, char **colName) {
    return 0;
}

int main() {
    sqlite3 *db = nullptr;
    sqlite3_open("/home/zhc/doc.db", &db);
    int a = sqlite3_exec(db, "select * from info", checkIfExistCallback, nullptr, nullptr);
    sqlite3_close(db);
}