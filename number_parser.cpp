#include "utils.hpp"

namespace properties {
    static const char *lowerCharTable = "0123456789abcdefghijklmnopqrstuvwxyz";
    static const char *upperCharTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static char mapTable[128];

    void init() {
        for (int i = 0; i < 36; ++i) {
            char c1 = lowerCharTable[i], c2 = upperCharTable[i];
            mapTable[c1] = (char) i, mapTable[c2] = (char) i;
        }
    }
}

int bczhc::utils::parseInt(const String &s, int radix) {
    properties::init();
    String numStr = s;
    if (s.isNull()) throw String("null");
    bool negative = s.charAt(0) == '-';
    if (negative) numStr = s.substring(1, s.length());
    int size = numStr.length();
    if (size == 0) return 0;

    int r = 0;
    for (int i = 0; i < size; ++i) {
        char c = numStr.charAt(size - i - 1);
        r += pow(radix, i) * properties::mapTable[c];
    }
    return negative ? -r : r;
}
