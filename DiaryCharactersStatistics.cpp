#include <algorithm>
#include <iostream>
#include "CountCharacters.h"
#include <iterator>
#include <map>
#include <sqlite3.h>
#include <vector>
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

bool cmp(pair<int, int64_t> &m1, pair<int, int64_t> &m2) {
    return m1.second < m2.second;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        ::cout << "cmd [diaryDatabasePath] [resultDatabasePath]" << ::endl;
        return 0;
    }
    sqlite3 *resultDatabase = nullptr;
    if (sqlite3_open(argv[2], &resultDatabase)) {
        ::cout << "Open or create result database error." << ::endl;
        return 1;
    }
    sqlite3_exec(resultDatabase, "CREATE TABLE IF NOT EXISTS chars(character TEXT, count INTEGER);", nullptr,
            nullptr, nullptr);
    sqlite3 *diaryDatabase = nullptr;
    if (sqlite3_open(argv[1], &diaryDatabase)) {
        ::cout << "Open diary database failed." << ::endl;
        return 1;
    }
    counter = new CharacterCounter;
    sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, nullptr, nullptr);
    map<int, int64_t> *data = counter->data;
    auto *vec = new vector<pair<int, int64_t>>(data->begin(), data->end());
    sort(vec->begin(), vec->end(), cmp);
    int t = vec->size();
    json *j = new json;
    char u8Char[5];
    sqlite3_exec(resultDatabase, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    for (int i = 0; i < t; ++i) {
        vector<pair<int, int64_t>> &v = *vec;
        int size = getUTF8Size(v[i].first);
        unicode2UTF8(u8Char, v[i].first), u8Char[size] = '\0';
        char *countNumStr = nullptr;
        char *cmd = nullptr;
        m_ltoa(&countNumStr, v[i].second);
        strcpyAndCat_auto(&cmd, "INSERT INTO chars VALUES('", -1, u8Char, -1, false);
        strcpyAndCat_auto(&cmd, cmd, -1, "',", -1, true);
        strcpyAndCat_auto(&cmd, cmd, -1, countNumStr, -1, true);
        strcpyAndCat_auto(&cmd, cmd, -1, ");", -1, true);
        sqlite3_exec(resultDatabase, cmd, nullptr, nullptr, nullptr);
        delete cmd, delete countNumStr;
        json arr;
        arr[0] = u8Char, arr[1] = v[i].second;
        (*j)[i] = arr;
    }
    sqlite3_exec(resultDatabase, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_close(diaryDatabase);
    sqlite3_close(resultDatabase);
    delete vec;
    string jsonStr = j->dump();
    delete j;
    cout << jsonStr << endl;
    return 0;
}
