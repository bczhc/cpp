#include <iostream>
#include "../Sqlite3.h"

using namespace std;
using namespace bczhc;
int main(int argc, char **argv) {
    Sqlite3 db;
    try {
        db.open("");
    } catch (...) {
        cout << "Open database error." << endl;
        return 0;
    }
    return 0;
}