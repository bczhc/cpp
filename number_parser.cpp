#include "utils.hpp"

static const char *charTable = "0123456789abcdefghijklmnopqrstuvwxyz";
static char mapTable[128];
void init() {
    for (int i = 0; i < 36; ++i) {
        char c = charTable[i];
        mapTable[c] = i;
    }
}

int pow(int base, int exponent) {
    int r = 1;
    for (int i = 0; i < exponent; ++i) {
        r *= base;
    }
    return r;
}

int bczhc::utils::parseInt(const String &s, int radix) {
    init();
    String numStr = s;
    if (s.isNull()) throw String("null");
    bool negative = s.charAt(0) == '-';
    if (negative) numStr = s.substring(1, s.size());
    int size = numStr.size();
    if (size == 0) return 0;

    int r = 0;
    for (int i = 0; i < size; ++i) {
        char c = numStr.charAt(size - i - 1);
        r += pow(radix, i) * mapTable[c];
    }
    return negative ? -r : r;
}

