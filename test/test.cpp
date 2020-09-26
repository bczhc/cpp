#include "../Sqlite3.hpp"

using namespace bczhc;

int main() {
    try {
        Sqlite3 db("/home/zhc/diary.db");
        class C : public Sqlite3::SqliteCallback {
        public:
            int callback(void *arg, int colNum, char **content, char **colName) override {
                printf("%s %i\n", content[0], *(int *) arg);
                return 0;
            }
        } callback;
        int a = 324;
        db.exec("SELECT * FROM diary", callback, &a);
    } catch (...) {
        printf("Something went wrong.");
    }
}
