#include "../third_party/practice/LinearList.hpp"
#include "../utf8.h"
#include <cstring>
#include <iostream>

using namespace bczhc;
using namespace utf8;
using namespace linearlist;

using namespace std;

wchar_t *utf8ToWChar(const char *s) {
    SequentialList<wchar_t> list{};
    SolvedUTF8Properties solved{};
    int offset = 0;
    while (s[offset] != '\0') {
        solveUTF8Bytes(solved, s + offset);
        offset += solved.bytesLength;
        if (solved.codepoint < 0xFFFF) {
            list.insert((wchar_t) solved.codepoint);
        } else {
            int t = solved.codepoint - 0x10000;
            list.insert((wchar_t)((t >> 10) & 0b1111111111) + 0xD800);
            list.insert((wchar_t)((t & 0b1111111111) + 0xDC00));
        }
    }
    int len = list.length();
    auto *r = new wchar_t[len + 1];
    r[len] = '\0';
    for (int i = 0; i < len; ++i) {
        r[i] = list.get(i);
    }
    return r;
}

int main() {
    wchar_t *r = utf8ToWChar("好现在是测试的，hello, world𪻐hhh");
    return 0;
}