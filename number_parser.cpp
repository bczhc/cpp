#include "utils.hpp"

class CharacterData {
public:
    virtual int digit(int ch, int radix) = 0;

    virtual ~CharacterData() = default;

    static CharacterData *of(int ch);
};

class CharacterDataLatin1 : public CharacterData {
private:
    int A[256] = {0};
    char *B;
public:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-string-literal-with-embedded-nul"

    CharacterDataLatin1() {
        String A_DATA = "\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800";
        A_DATA = A_DATA + "\u100F\u4800\u100F\u4800\u100F\u5800\u400F\u5000\u400F\u5800\u400F\u6000\u400F" +
                 "\u5000\u400F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800" +
                 "\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F" +
                 "\u4800\u100F\u4800\u100F\u5000\u400F\u5000\u400F\u5000\u400F\u5800\u400F\u6000" +
                 "\u400C\u6800\030\u6800\030\u2800\030\u2800\u601A\u2800\030\u6800\030\u6800" +
                 "\030\uE800\025\uE800\026\u6800\030\u2000\031\u3800\030\u2000\024\u3800\030" +
                 "\u3800\030\u1800\u3609\u1800\u3609\u1800\u3609\u1800\u3609\u1800\u3609\u1800" +
                 "\u3609\u1800\u3609\u1800\u3609\u1800\u3609\u1800\u3609\u3800\030\u6800\030" +
                 "\uE800\031\u6800\031\uE800\031\u6800\030\u6800\030\202\u7FE1\202\u7FE1\202" +
                 "\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1" +
                 "\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202" +
                 "\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1\202\u7FE1" +
                 "\202\u7FE1\uE800\025\u6800\030\uE800\026\u6800\033\u6800\u5017\u6800\033\201" +
                 "\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2" +
                 "\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201" +
                 "\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2\201\u7FE2" +
                 "\201\u7FE2\201\u7FE2\201\u7FE2\uE800\025\u6800\031\uE800\026\u6800\031\u4800" +
                 "\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u5000\u100F" +
                 "\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800" +
                 "\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F" +
                 "\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800" +
                 "\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F\u4800\u100F" +
                 "\u3800\014\u6800\030\u2800\u601A\u2800\u601A\u2800\u601A\u2800\u601A\u6800" +
                 "\034\u6800\030\u6800\033\u6800\034\000\u7005\uE800\035\u6800\031\u4800\u1010" +
                 "\u6800\034\u6800\033\u2800\034\u2800\031\u1800\u060B\u1800\u060B\u6800\033" +
                 "\u07FD\u7002\u6800\030\u6800\030\u6800\033\u1800\u050B\000\u7005\uE800\036" +
                 "\u6800\u080B\u6800\u080B\u6800\u080B\u6800\030\202\u7001\202\u7001\202\u7001" +
                 "\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202" +
                 "\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001" +
                 "\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\u6800\031\202\u7001\202" +
                 "\u7001\202\u7001\202\u7001\202\u7001\202\u7001\202\u7001\u07FD\u7002\201\u7002" +
                 "\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201" +
                 "\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002" +
                 "\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\u6800" +
                 "\031\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002\201\u7002" +
                 "\u061D\u7002";

        String B_S = "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000";
        B_S = B_S +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000" +
              "\000\000\000\000\000\000\000\000\000";
        B = B_S.toCharArray();

        char *data = A_DATA.toCharArray();
        int i = 0, j = 0;
        while (i < (256 * 2)) {
            int entry = data[i++] << 16;
            A[j++] = entry | data[i++];
        }
        delete data;
    }

    ~CharacterDataLatin1() override {
        delete[] B;
    }

#pragma clang diagnostic pop

    int getProperties(int ch) {
        char offset = (char) ch;
        int props = A[offset];
        return props;
    }

    int digit(int ch, int radix) override {
        int value = -1;
        if (radix >= 2 && radix <= 36) {
            int val = getProperties(ch);
            int kind = val & 0x1F;
            if (kind == 9) {
                value = ch + ((val & 0x3E0) >> 5) & 0x1F;
            } else if ((val & 0xC00) == 0x00000C00) {
                // Java supradecimal digit
                value = (ch + ((val & 0x3E0) >> 5) & 0x1F) + 10;
            }
        }
        return (value < radix) ? value : -1;
    }
};

String forInputString(const String &s) {
    String msg = "For input string: \"";
    msg.append(s).append('\"');
    return msg;
}

CharacterData *CharacterData::of(int ch) {
    return new CharacterDataLatin1;
}

namespace func {
    int digit(int codepoint, int radix) {
        CharacterData *pCharacterData = CharacterData::of(codepoint);
        int r = pCharacterData->digit(codepoint, radix);
        delete pCharacterData;
        return r;
    }
}

int bczhc::utils::parseInt(const String &s, int radix) {
    if (s.isNull()) {
        throw String("null");
    }

    if (radix < 2) {
        String msg = "radix ";
        msg.append(String::toString(radix))
                .append(" less than Character.MIN_RADIX");
        throw msg;
    }

    if (radix > 36) {
        String msg = "radix ";
        msg.append(String::toString(radix))
                .append(" greater than Character.MAX_RADIX");
        throw msg;
    }

    int result = 0;
    bool negative = false;
    int i = 0, len = s.size();
    int limit = -0x7fffffff;
    int multmin;
    int digit;

    if (len > 0) {
        char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = (int) 0x80000000;
            } else if (firstChar != '+') {
                throw forInputString(s);
            }


            // Cannot have lone "+" or "-"
            if (len == 1) {
                throw forInputString(s);
            }
            i++;
        }
        multmin = limit / radix;
        while (i < len) {
            // Accumulating negatively avoids surprises near MAX_VALUE
            digit = func::digit(s.charAt(i++), radix);
            if (digit < 0) {
                throw forInputString(s);
            }
            if (result < multmin) {
                throw forInputString(s);
            }
            result *= radix;
            if (result < limit + digit) {
                throw forInputString(s);
            }
            result -= digit;
        }
    } else {
        throw forInputString(s);
    }
    return negative ? result : -result;
}