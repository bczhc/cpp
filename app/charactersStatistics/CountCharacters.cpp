#include <cstring>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by root on 2020/8/23.
//

#include "CountCharacters.h"
#include <iostream>

using namespace std;

bczhc::CharacterCounter::CharacterCounter() { data = new map<int, int64_t>; }

bczhc::CharacterCounter::~CharacterCounter() { delete data; }

int bczhc::CharacterCounter::countCharacters(const char *u8Bytes,
                                             int size) const {
    if (size == -1)
        size = strlen(u8Bytes);
    int offset = 0;
    SolvedUTF8Properties solvedProperties{};
    while (offset < size) {
        solveUTF8Bytes(solvedProperties, u8Bytes + offset);
        int codepoint = solvedProperties.codepoint;
        offset += solvedProperties.bytesLength;
        if (data->find(codepoint) == data->end()) {
            (*data)[codepoint] = 1;
        } else {
            (*data)[codepoint] = data->find(codepoint)->second + 1;
        }
    }
    return 0;
}

bool cmp(pair<int, int64_t> &m1, pair<int, int64_t> &m2) {
    return m1.second < m2.second;
}

using vec = vector<pair<int, int64_t>>;

vec *bczhc::CharacterCounter::getSortVector() {
    auto *v = new vector<pair<int, int64_t>>(data->begin(), data->end());
    std::sort(v->begin(), v->end(), cmp);
    return v;
}

json *bczhc::CharacterCounter::getJsonData() {
    vec *v = getSortVector();
    json *j = new json;
    int size = v->size();
    char u8Char[5];
    int codepoint;
    for (int i = 0; i < size; ++i) {
        json pairJson;
        codepoint = (*v)[i].first;
        unicode2UTF8(u8Char, codepoint),
                u8Char[getUTF8Size(codepoint)] = '\0';
        pairJson[0] = u8Char, pairJson[1] = (*v)[i].second;
        (*j)[i] = pairJson;
    }
    return j;
}

#pragma clang diagnostic pop
