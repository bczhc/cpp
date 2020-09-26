#include "../CountCharacters.h"
#include "../third_party/json-single-header/single_include/nlohmann/json.hpp"
#include "../third_party/sqlite3-single-c/sqlite3.h"
#include "../utf8.h"
#include "../zhc.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
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

inline bool range(int codepoint) {
    if (codepoint >= 0x4e00 && codepoint <= 0x9fff)
        return true;
    if (codepoint >= 0x3400 && codepoint <= 0x4dbf)
        return true;
    if (codepoint >= 0x20000 && codepoint <= 0x2a6df)
        return true;
    if (codepoint >= 0x2a700 && codepoint <= 0x2b73f)
        return true;
    if (codepoint >= 0x2b740 && codepoint <= 0x2b81f)
        return true;
    if (codepoint >= 0x2b820 && codepoint <= 0x2ceaf)
        return true;
    if (codepoint >= 0xf900 && codepoint <= 0xfaff)
        return true;
    if (codepoint >= 0x2f800 && codepoint <= 0x2fa1f)
        return true;
    return false;
}

void han() {
    int c = 0;
    using vec = vector<pair<int, int64_t>>;
    vec *v = counter->getSortVector();
    int len = v->size();
    char u8Char[5];
    for (int i = len - 1; i >= 0; --i) {
        int codepoint = (*v)[i].first;
        if (!range(codepoint))
            continue;
        unicode2UTF8(u8Char, codepoint);
        u8Char[getUTF8Size(codepoint)] = '\0';
        cout << u8Char;
        ++c;
        if (c % 25 == 0) cout << endl;
    }
    cout << endl
         << "Total: " << c << endl;
    delete v;
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
    han();
    return 0;
    json *j = counter->getJsonData();
    cout << j->dump() << endl;
    delete j;
    return 0;
}
