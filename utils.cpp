//
// Created by zhc on 11/21/20.
//

#include "utils.hpp"

int64_t bczhc::utils::getCurrentTimeMillis() {
    timeval t{};
    gettimeofday(&t, nullptr);
    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

int bczhc::utils::min(int a, int b) {
    return a < b ? a : b;
}

int bczhc::utils::pow(int base, int exponent) {
    int r = 1;
    for (int i = 0; i < exponent; ++i) {
        r *= base;
    }
    return r;
}

int8_t bczhc::utils::lsl(int8_t x, uint8_t n) {
    return (int8_t) (((uint8_t) x) << n);
}

int8_t bczhc::utils::lsr(int8_t x, uint8_t n) {
    return (int8_t) (((uint8_t) x) >> n);
}

int16_t bczhc::utils::lsl(int16_t x, uint16_t n) {
    return (int16_t) (((uint16_t) x) << n);
}

int16_t bczhc::utils::lsr(int16_t x, uint16_t n) {
    return (int16_t) (((uint16_t) x) >> n);
}

int64_t bczhc::utils::lsl(int64_t x, uint64_t n) {
    return (int64_t) (((uint64_t) x) << n);
}

int32_t bczhc::utils::lsl(int32_t x, uint32_t n) {
    return (int32_t) (((uint32_t) x) << n);
}

int32_t bczhc::utils::lsr(int32_t x, uint32_t n) {
    return (int32_t) (((uint32_t) x) >> n);
}

int64_t bczhc::utils::lsr(int64_t x, uint64_t n) {
    return (int64_t) (((uint64_t) x) >> n);
}

bczhc::utils::Endianness bczhc::utils::getEndianness() {
    int16_t a = 0x0102;
    if (*((int8_t *) &a) == 0x02) return Endianness::LITTLE;
    return Endianness::BIG;
}

int32_t bczhc::utils::Long::stringSize(int64_t x) {
    int64_t p = 10;
    for (int32_t i = 1; i < 19; i++) {
        if (x < p) return i;
        p = 10 * p;
    }
    return 19;
}

void bczhc::utils::Long::getChars(int64_t i, int32_t index, char *buf) {
    int64_t q;
    int32_t r;
    int32_t charPos = index;
    char sign = 0;

    if (i < 0) {
        sign = '-';
        i = -i;
    }

    // Get 2 digits/iteration using longs until quotient fits into an int
    while (i > Integer::MAX_VALUE) {
        q = i / 100;
        // really: r = i - (q * 100);
        r = (int32_t) (i - ((lsl(q, 6)) + (lsl(q, 5)) + (lsl(q, 2))));
        i = q;
        buf[--charPos] = Integer::DigitOnes[r];
        buf[--charPos] = Integer::DigitTens[r];
    }

    // Get 2 digits/iteration using ints
    int32_t q2;
    auto i2 = (int32_t) i;
    while (i2 >= 65536) {
        q2 = i2 / 100;
        // really: r = i2 - (q * 100);
        r = i2 - ((lsl(q2, 6)) + (lsl(q2, 5)) + (lsl(q2, 2)));
        i2 = q2;
        buf[--charPos] = Integer::DigitOnes[r];
        buf[--charPos] = Integer::DigitTens[r];
    }

    // Fall thru to fast mode for smaller numbers
    // assert(i2 <= 65536, i2);
    for (;;) {
        q2 = lsr(i2 * 52429, 16 + 3);
        r = i2 - ((lsl(q2, 3)) + (lsl(q2, 1)));  // r = i2-(q2*10) ...
        buf[--charPos] = Integer::digits[r];
        i2 = q2;
        if (i2 == 0) break;
    }
    if (sign != 0) {
        buf[charPos - 1] = sign;
    }
}

String bczhc::utils::Long::toString(int64_t i) {
    if (i == MIN_VALUE) return "-9223372036854775808";
    int32_t size = (i < 0) ? stringSize(-i) + 1 : stringSize(i);
    char buf[size];
    getChars(i, size, buf);
    return String(buf, size);
}

int64_t bczhc::utils::Long::javaParseLong(const String &s, int radix) {
    if (s.isNull()) throw NumberFormatException("null");
    
    if (radix > 36 || radix < 2) throw NumberFormatException("illegal radix");

    bool negative = false;
    int32_t i = 0, len = s.length();
    int64_t limit = -Long::MAX_VALUE;

    if (len > 0) {
        char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN_VALUE;
            } else if (firstChar != '+') {
                throw NumberFormatException::forInputString(s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException::forInputString(s);
            }
            i++;
        }
        int64_t multmin = limit / radix;
        int64_t result = 0;
        while (i < len) {
            // Accumulating negatively avoids surprises near MAX_VALUE
            int32_t digit = Character::digit(s.charAt(i++), radix);
            if (digit < 0 || result < multmin) {
                throw NumberFormatException::forInputString(s);
            }
            result *= radix;
            if (result < limit + digit) {
                throw NumberFormatException::forInputString(s);
            }
            result -= digit;
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException::forInputString(s);
    }
}

int64_t bczhc::utils::Long::javaParseLong(const String &s) {
    return javaParseLong(s, 10);
}

int64_t bczhc::utils::Long::javaParseUnsignedLong(const String &s) {
    return javaParseUnsignedLong(s, 10);
}

int64_t bczhc::utils::Long::javaParseUnsignedLong(const String &s, int32_t radix) {
    if (s.isNull()) throw NumberFormatException("null");

    int32_t len = s.length();
    if (len > 0) {
        char firstChar = s.charAt(0);
        if (firstChar == '-') {
            String msg = "Illegal leading minus sign ";
            msg += "on unsigned string ";
            msg += s + '.';
            throw NumberFormatException(s);
        } else {
            if (len <= 12 || // Long.MAX_VALUE in Character.MAX_RADIX is 13 digits
                (radix == 10 && len <= 18)) { // Long.MAX_VALUE in base 10 is 19 digits
                return javaParseLong(s, radix);
            }

            // No need for range checks on len due to testing above.
            int64_t first = javaParseLong(s, 0, len - 1, radix);
            int32_t second = Character::digit(s.charAt(len - 1), radix);
            if (second < 0) {
                throw NumberFormatException(String("Bad digit at end of ") + s);
            }
            int64_t result = first * radix + second;

            /*
             * Test leftmost bits of multiprecision extension of first*radix
             * for overflow. The number of bits needed is defined by
             * GUARD_BIT = ceil(log2(Character.MAX_RADIX)) + 1 = 7. Then
             * int guard = radix*(int)(first >>> (64 - GUARD_BIT)) and
             * overflow is tested by splitting guard in the ranges
             * guard < 92, 92 <= guard < 128, and 128 <= guard, where
             * 92 = 128 - Character.MAX_RADIX. Note that guard cannot take
             * on a value which does not include a prime factor in the legal
             * radix range.
             */
            int32_t guard = radix * (int32_t) (lsr(first, 57));
            if (guard >= 128 ||
                (result >= 0 && guard >= 128 - Character::MAX_RADIX)) {
                /*
                 * For purposes of exposition, the programmatic statements
                 * below should be taken to be multi-precision, i.e., not
                 * subject to overflow.
                 *
                 * A) Condition guard >= 128:
                 * If guard >= 128 then first*radix >= 2^7 * 2^57 = 2^64
                 * hence always overflow.
                 *
                 * B) Condition guard < 92:
                 * Define left7 = first >>> 57.
                 * Given first = (left7 * 2^57) + (first & (2^57 - 1)) then
                 * result <= (radix*left7)*2^57 + radix*(2^57 - 1) + second.
                 * Thus if radix*left7 < 92, radix <= 36, and second < 36,
                 * then result < 92*2^57 + 36*(2^57 - 1) + 36 = 2^64 hence
                 * never overflow.
                 *
                 * C) Condition 92 <= guard < 128:
                 * first*radix + second >= radix*left7*2^57 + second
                 * so that first*radix + second >= 92*2^57 + 0 > 2^63
                 *
                 * D) Condition guard < 128:
                 * radix*first <= (radix*left7) * 2^57 + radix*(2^57 - 1)
                 * so
                 * radix*first + second <= (radix*left7) * 2^57 + radix*(2^57 - 1) + 36
                 * thus
                 * radix*first + second < 128 * 2^57 + 36*2^57 - radix + 36
                 * whence
                 * radix*first + second < 2^64 + 2^6*2^57 = 2^64 + 2^63
                 *
                 * E) Conditions C, D, and result >= 0:
                 * C and D combined imply the mathematical result
                 * 2^63 < first*radix + second < 2^64 + 2^63. The lower
                 * bound is therefore negative as a signed long, but the
                 * upper bound is too small to overflow again after the
                 * signed long overflows to positive above 2^64 - 1. Hence
                 * result >= 0 implies overflow given C and D.
                 */
                String msg = "String value ";
                msg += s, msg += " exceeds ", msg += "range of unsigned long.";
                throw NumberFormatException(msg);
            }
            return result;
        }
    } else {
        throw NumberFormatException::forInputString(s);
    }
}

int64_t bczhc::utils::Long::javaParseLong(const String &s, int32_t beginIndex, int32_t endIndex, int32_t radix) {
    if (s.isNull()) throw NullPointerException();
    if (beginIndex < 0 || beginIndex > endIndex || endIndex > s.length()) {
        throw IndexOutOfBoundsException();
    }
    if (radix < Character::MIN_RADIX || radix > Character::MAX_RADIX) {
        throw NumberFormatException("illegal radix");
    }

    bool negative = false;
    int32_t i = beginIndex;
    int64_t limit = -Long::MAX_VALUE;

    if (i < endIndex) {
        char firstChar = s.charAt(i);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN_VALUE;
            } else if (firstChar != '+') {
                throw NumberFormatException::forCharSequence(s, beginIndex, endIndex, i);
            }
            i++;
        }
        if (i >= endIndex) { // Cannot have lone "+", "-" or ""
            throw NumberFormatException::forCharSequence(s, beginIndex, endIndex, i);
        }
        int64_t multmin = limit / radix;
        int64_t result = 0;
        while (i < endIndex) {
            // Accumulating negatively avoids surprises near MAX_VALUE
            int32_t digit = Character::digit(s.charAt(i), radix);
            if (digit < 0 || result < multmin) {
                throw NumberFormatException::forCharSequence(s, beginIndex, endIndex, i);
            }
            result *= radix;
            if (result < limit + digit) {
                throw NumberFormatException::forCharSequence(s, beginIndex, endIndex, i);
            }
            i++;
            result -= digit;
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException("");
    }
}

int64_t bczhc::utils::Long::parseLong(const String &s, int32_t radix) {
    //for negative binary
    if (radix == 2) return javaParseUnsignedLong(s, 2);
    return javaParseUnsignedLong(s, radix);
}

int64_t bczhc::utils::Long::parseLong(const String &s) {
    return parseLong(s, 10);
}

int32_t bczhc::utils::Integer::javaParseUnsignedInt(const String &s, int32_t radix) {
    if (s.isNull()) throw NumberFormatException("null");
    int32_t len = s.length();
    if (len > 0) {
        char firstChar = s.charAt(0);
        if (firstChar == '-') {
            String msg = String("Illegal leading minus sign ") + "on unsigned string ";
            msg += s + '.';
            throw NumberFormatException(s);
        } else {
            if (len <= 5 || // Integer.MAX_VALUE in Character.MAX_RADIX is 6 digits
                (radix == 10 && len <= 9)) { // Integer.MAX_VALUE in base 10 is 10 digits
                return javaParseInt(s, radix);
            } else {
                int64_t ell = Long::javaParseLong(s, radix);
                if (((int64_t) (((uint64_t) ell) & 0xffffffff00000000LU)) == 0) {
                    return (int) ell;
                } else {
                    String msg = "String value ";
                    msg += s;
                    msg += " exceeds ";
                    msg += "range of unsigned int.";
                    throw NumberFormatException(msg);
                }
            }
        }
    } else {
        throw NumberFormatException::forInputString(s);
    }
}

int32_t bczhc::utils::Integer::javaParseInt(const String &s, int32_t radix) {
    if (s.isNull()) throw NumberFormatException("null");
    if (radix > 36 || radix < 2) throw NumberFormatException("illegal radix");
    bool negative = false;
    int32_t i = 0, len = s.length();
    int32_t limit = -Integer::MAX_VALUE;

    if (len > 0) {
        char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Integer::MIN_VALUE;
            } else if (firstChar != '+') {
                throw NumberFormatException::forInputString(s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException::forInputString(s);
            }
            i++;
        }
        int32_t multmin = limit / radix;
        int32_t result = 0;
        while (i < len) {
            // Accumulating negatively avoids surprises near MAX_VALUE
            int32_t digit = Character::digit(s.charAt(i++), radix);
            if (digit < 0 || result < multmin) {
                throw NumberFormatException::forInputString(s);
            }
            result *= radix;
            if (result < limit + digit) {
                throw NumberFormatException::forInputString(s);
            }
            result -= digit;
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException::forInputString(s);
    }
}

String bczhc::utils::Integer::toString(int32_t i) {
    if (i == MIN_VALUE) return "-2147483648";
    int32_t size = (i < 0) ? stringSize(-i) + 1 : stringSize(i);
    char buf[size];
    getChars(i, size, buf);
    return String(buf, size);
}

void bczhc::utils::Integer::getChars(int32_t i, int32_t index, char *buf) {
    int32_t q, r;
    int32_t charPos = index;
    char sign = 0;

    if (i < 0) {
        sign = '-';
        i = -i;
    }

    // Generate two digits per iteration
    while (i >= 65536) {
        q = i / 100;
        // really: r = i - (q * 100);
        r = i - ((lsl(q, 6)) + (lsl(q, 5)) + (lsl(q, 2)));
        i = q;
        buf[--charPos] = DigitOnes[r];
        buf[--charPos] = DigitTens[r];
    }

    // Fall thru to fast mode for smaller numbers
    // assert(i <= 65536, i);
    for (;;) {
        q = lsr(i * 52429, 16 + 3);
        r = i - ((lsl(q, 3)) + (lsl(q, 1)));  // r = i-(q*10) ...
        buf[--charPos] = digits[r];
        i = q;
        if (i == 0) break;
    }
    if (sign != 0) {
        buf[charPos - 1] = sign;
    }
}

int32_t bczhc::utils::Integer::stringSize(int32_t x) {
    for (int32_t i = 0;; i++)
        if (x <= sizeTable[i])
            return i + 1;
}

int32_t bczhc::utils::Integer::javaParseUnsignedInt(const String &s) {
    return javaParseUnsignedInt(s, 10);
}

int32_t bczhc::utils::Integer::javaParseInt(const String &s) {
    return javaParseInt(s, 10);
}

int32_t bczhc::utils::Integer::parseInt(const String &s, int32_t radix) {
    // for negative binary
    if (radix == 2) return javaParseUnsignedInt(s, 2);
    return javaParseInt(s, radix);
}

int32_t bczhc::utils::Integer::parseInt(const String &s) {
    return parseInt(s, 10);
}

int32_t bczhc::utils::Character::digit(int32_t ch, int32_t radix) {
    int value = DIGITS[ch];
    return (value >= 0 && value < radix && radix >= 2
            && radix <= 36) ? value : -1;
}
