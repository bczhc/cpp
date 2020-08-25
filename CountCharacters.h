//
// Created by root on 2020/8/23.
//

#ifndef CPP_COUNTCHARACTERS_H
#define CPP_COUNTCHARACTERS_H

#include <sqlite3.h>
#include <map>
#include <cstdint>
#include "third_party/json/single_include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace bczhc {
    class CharacterCounter {
        public:
            map<int, int64_t> *data;

            CharacterCounter();

            ~CharacterCounter();

            int countCharacters(const char *u8Bytes, int size) const;

            json *getJsonData();
    };
}

#endif //CPP_COUNTCHARACTERS_H
