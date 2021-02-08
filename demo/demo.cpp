#include "../concurrent.h"
#include "../file.h"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.h"
#include "../array.hpp"
#include "../def.h"
#include <cassert>
#include "../app/base128/Base128Lib.h"

extern "C" {
#include "../third_party/crypto-algorithms/sha256.h"
}

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace file;
using namespace utils;
using namespace io;
using namespace array;

Array<uchar> sha256Encode(uchar *data, size_t size) {
    assert(sizeof(uchar) == sizeof(BYTE));
    Array<uchar> buf(SHA256_BLOCK_SIZE);
    SHA256_CTX ctx{};
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE *) data, size);
    sha256_final(&ctx, buf.elements);
    return buf;
}

Array<uchar> sha256Encode(const String &str) {
    return sha256Encode((uchar *) str.getCString(), str.length());
}

String hexArrToStr(const uchar *a, size_t size) {
    String s;
    for (size_t i = 0; i < size; ++i) {
        String hex = String::toString(a[i], 16);
        if (hex.length() == 1) hex = String("0") + hex;
        s += hex;
    }
    return s;
}

String sha256EncodeToString(const String &str) {
    const Array<uchar> a = sha256Encode(str);
    return hexArrToStr(a.elements, a.length());
}

String encrypt(const String &str) {
    auto s = sha256EncodeToString(str);
    String s2;
    for (int i = (int) s.length() - 1; i >= 0; --i) {
        s2 += s[i];
    }
    String salt = "bczhc";
    s2 += salt;
    auto a = sha256Encode(s2);
    Array<uchar> b(a.length() + 3);
    int i;
    for (i = 0; i < a.length(); ++i) {
        b[i] = a[i];
    }
    char c[] = {0x12, 0x34, 0x56};
    int o = i;
    for (; i < b.length(); ++i) {
        b[i] = c[i - o];
    }
    Array<uchar> d(40);
    for (i = 0; i < b.length() / 7; ++i) base128::encode7bytes(d.elements + i * 8, b.elements + i * 7);
    return sha256EncodeToString(hexArrToStr(d.elements, d.length()) + salt);
}

int main() {
    InputStream is(stdin);
    LineReader reader(is);
    while (true) {
        cout << "enter: ";
        const String line = reader.readLine();
        if (line.isNull()) break;
        cout << "\033[1;33m";
        cout << encrypt(line).getCString() << endl;
        cout << "\033[0m";
    }
    return 0;
}