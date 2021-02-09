//
// Created by root on 2020/8/23.
//

#ifndef CPP_COUNTCHARACTERS_H
#define CPP_COUNTCHARACTERS_H

#include "../../third_party/json-single-header/single_include/nlohmann/json.hpp"
#include "../../third_party/sqlite3-single-c/sqlite3.h"
#include <cstdint>
#include <map>
#include <vector>
#include "../../string.hpp"

using namespace std;
using json = nlohmann::json;
using namespace bczhc;

namespace bczhc {
    class CharacterCounter {
    public:
        map<int, int64_t> *data;

        CharacterCounter();

        ~CharacterCounter();

        int countCharacters(const char *u8Bytes, int size) const;

        vector<pair<int, int64_t>> *getSortVector();

        json *getJsonData();
    };
}// namespace bczhc

#endif// CPP_COUNTCHARACTERS_H
