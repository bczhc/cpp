#include <cstring>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by root on 2020/8/23.
//

#include "CountCharacters.h"
#include "utf8.h"

using namespace std;

CharacterCounter::CharacterCounter() {
    data = new map<int, int64_t>;
}

CharacterCounter::~CharacterCounter() {
    delete data;
}

int CharacterCounter::countCharacters(const char *u8Bytes, int size) const {
    if (size == -1) size = strlen(u8Bytes);
    int offset = 0;
    bczhc::utf8::SolvedUTF8Properties solvedProperties{};
    while (offset < size) {
        bczhc::utf8::solveUTF8Bytes(solvedProperties, u8Bytes + offset);
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

#pragma clang diagnostic pop
