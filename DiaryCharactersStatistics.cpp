#include <iostream>
#include "CountCharacters.h"
#include <map>
#include <sqlite3.h>
#include "utf8.h"
#include "zhc.h"
#include "third_party/json/single_include/nlohmann/json.hpp"

using namespace std;
using namespace bczhc;
using namespace utf8;

using json = nlohmann::json;

CharacterCounter *counter;

int callback(void *arg, int colNum, char **colVal, char **colName) {
    counter->countCharacters(colVal[0], -1);
    return 0;
}

int main() {
    sqlite3 *resultDatabase = nullptr;
    if (sqlite3_open("/home/zhc/code/code/C++14/result.db", &resultDatabase)) {
        ::cout << "Open or create result database error." << ::endl;
        return 1;
    }
    sqlite3_exec(resultDatabase, "CREATE TABLE IF NOT EXISTS chars(character TEXT, count INTEGER);", nullptr,
                 nullptr, nullptr);
    sqlite3 *diaryDatabase = nullptr;
    if (sqlite3_open("/home/zhc/code/code/C++14/diary.db", &diaryDatabase)) {
        ::cout << "Open diary database failed." << ::endl;
        return 1;
    }
    counter = new CharacterCounter;
    sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, nullptr, nullptr);
    auto it = counter->data->begin();
    char u8Char[5];
    sqlite3_exec(resultDatabase, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    json j;
    int i = 0;
    for (; it != counter->data->end(); it++, ++i) {
        int size = getUTF8Size(it->first);
        unicode2UTF8(u8Char, it->first), u8Char[size] = '\0';
        char *countNumStr = nullptr;
        char *cmd = nullptr;
        m_ltoa(&countNumStr, it->second);
        strcpyAndCat_auto(&cmd, "INSERT INTO chars VALUES('", -1, u8Char, -1, false);
        strcpyAndCat_auto(&cmd, cmd, -1, "',", -1, true);
        strcpyAndCat_auto(&cmd, cmd, -1, countNumStr, -1, true);
        strcpyAndCat_auto(&cmd, cmd, -1, ");", -1, true);
        sqlite3_exec(resultDatabase, cmd, nullptr, nullptr, nullptr);
        delete cmd, delete countNumStr;
        json arr;
        arr[0] = u8Char, arr[1] = it->second;
        j[i] = arr;
    }
    sqlite3_exec(resultDatabase, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_close(diaryDatabase);
    sqlite3_close(resultDatabase);
    ::cout << j.dump() << ::endl;
    return 0;
}
