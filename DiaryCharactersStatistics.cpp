#include "CountCharacters.h"
#include "third_party/json/single_include/nlohmann/json.hpp"
#include "utf8.h"
#include "zhc.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sqlite3.h>
#include <vector>

using namespace std;
using namespace bczhc;
using namespace utf8;

using json = nlohmann::json;

CharacterCounter *counter;

int callback(void *arg, int colNum, char **colVal, char **colName) {
    counter->countCharacters(colVal[0], -1);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        ::cout << "cmd [diaryDatabasePath]" << ::endl;
        return 0;
    }
    sqlite3 *diaryDatabase = nullptr;
    if (sqlite3_open(argv[1], &diaryDatabase)) {
        ::cout << "Open diary database failed." << ::endl;
        return 1;
    }
    counter = new CharacterCounter;
    sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, nullptr,
                 nullptr);
    sqlite3_close(diaryDatabase);
    json *j = counter->getJsonData();
    ::cout << j->dump() << ::endl;
    delete j;
    return 0;
}
