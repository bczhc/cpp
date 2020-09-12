#include <sqlite3.h>

int main(int argc, char **argv) {
    const char *f;
    if (argc < 2) f = "/storage/emulated/0/diary.db";
    else f = argv[1];
    sqlite3 *db = nullptr;
    sqlite3_open(f, &db);

    

    sqlite3_close(db);
    return 0;
}
