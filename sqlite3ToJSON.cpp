#include <ios>
#include <sqlite3.h>
#include "./json/json.hpp"
#include <iostream>

using namespace std;
using JSON = nlohmann::json;

int main() {
    int status = 0;
    char *errmsg;
    sqlite3 *db = nullptr;
    status = sqlite3_open("/storage/emulated/0/doc.db", &db);
    if (status) ::cout << "Opening database failed." << ::endl, exit(1);
    ::JSON j;
    int i = 0;
    void *arg[] = {&j, &i};
    sqlite3_exec(db, "SELECT * FROM doc", [](void *arg, int col, char **fields, char **cols) -> int {
        JSON *json = (JSON *) (((void **) arg)[0]);
        int *c = (int *) (((void **) arg)[1]);
        ::JSON record;
        for (int i = 0; i < col; ++i) {
            record[cols[i]] = fields[i];
        }
        (*json)[(*c)++] = record;
        return 0;
    }, arg, &errmsg);
    sqlite3_free(errmsg);
    sqlite3_close(db);
    ::cout << j.dump(4) << ::endl;
    return 0;
}
