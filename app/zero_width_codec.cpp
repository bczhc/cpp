//
// Created by bczhc on 3/6/21.
//

#include "../string.hpp"
#include "../utils.hpp"
#include "../io.hpp"
#include <iostream>

using namespace bczhc;
using namespace std;

String encode(const String &s) {
    const char *m1 = "\u200b", *m2 = "\u200d";
    String encoded;
    for (int i = 0; i < s.length(); ++i) {
        char c = s[i];
        auto binStr = String::toString((int64_t) (uchar) c, 2);
        auto l = 8 - binStr.length();
        String t;
        for (int j = 0; j < l; ++j) {
            t += '0';
        }
        binStr = t + binStr;
        for (int j = 0; j < binStr.length(); ++j) {
            if (binStr[j] == '0') encoded += m1;
            else encoded += m2;
        }
    }
    return encoded;
}

String decode(const String &s) {
    uint32_t m1 = 0x200b, m2 = 0x200d;
    String decoded;
    size_t start = 0;
    SolvedUTF8Properties solved{0, 0};
    String binStr;
    for (int i = 0; start < s.length(); ++i, start += solved.bytesLength) {
        solveUTF8Bytes(solved, (uchar *) (s.getCString() + start));
        if (solved.codepoint == m1) binStr += '0';
        else if (solved.codepoint == m2) binStr += '1';
        if (i % 8 == 7) {
            char c = (char) Integer::parseInt(binStr, 2);
            decoded.append(c);
            binStr.clear();
        }
    }
    return decoded;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: Command (e|d)>" << endl;
        return 0;
    }
    InputStream is(stdin);
    String s;
    int readLen = 0;
    char buf[40960];
    while((readLen = is.read(buf, 40960)) > 0) {
        s.append(buf, readLen);
    }
    if (String::equal(argv[1], "e")) {
        cout << encode(s).getCString();
        cout.flush();
    } else if (String::equal(argv[1], "d")) {
        cout << decode(s).getCString();
        cout.flush();
    } else cout << (String("Unknown option: ") + argv[1]).getCString() << endl;
    return 0;
}