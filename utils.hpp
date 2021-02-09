//
// Created by zhc on 11/21/20.
//

#ifndef CPP_UTILS_HPP
#define CPP_UTILS_HPP

#include <cstdint>
#include <sys/time.h> // NOLINT(modernize-deprecated-headers)
#include <cstdio>
#include "./string.hpp"
#include <cmath>
#include "./exception.hpp"
#include "string.hpp"

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

using namespace bczhc;

namespace bczhc {
    enum Endianness {
        BIG, LITTLE
    };

    int min(int a, int b);

    int pow(int base, int exponent);

    template<typename T>
    class Result {
        int status = 0;
        T r;
    };

    int8_t lsl(int8_t x, uint8_t n);

    int8_t lsr(int8_t x, uint8_t n);

    int16_t lsl(int16_t x, uint16_t n);

    int16_t lsr(int16_t x, uint16_t n);

    int64_t lsl(int64_t x, uint64_t n);

    int32_t lsl(int32_t x, uint32_t n);

    int32_t lsr(int32_t x, uint32_t n);

    int64_t lsr(int64_t x, uint64_t n);

    class Character {
    private:
        static constexpr int32_t DIGITS[] = {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1,
                -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1, 10, 11, 12,
                13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                33, 34, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    public:
        static int32_t digit(int32_t ch, int32_t radix);

        static constexpr int32_t MIN_RADIX = 2;
        static constexpr int32_t MAX_RADIX = 36;
    };

    Endianness getEndianness();

    int64_t getCurrentTimeMillis();

    class Integer {
    public:
        static const int32_t MIN_VALUE = 0x80000000;
        static const int32_t MAX_VALUE = 0x7fffffff;

        constexpr static const char DigitOnes[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        };
        constexpr static const char DigitTens[] = {
                '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                '2', '2', '2', '2', '2', '2', '2', '2', '2', '2',
                '3', '3', '3', '3', '3', '3', '3', '3', '3', '3',
                '4', '4', '4', '4', '4', '4', '4', '4', '4', '4',
                '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
                '6', '6', '6', '6', '6', '6', '6', '6', '6', '6',
                '7', '7', '7', '7', '7', '7', '7', '7', '7', '7',
                '8', '8', '8', '8', '8', '8', '8', '8', '8', '8',
                '9', '9', '9', '9', '9', '9', '9', '9', '9', '9',
        };

        constexpr static const char digits[] = {
                '0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'a', 'b',
                'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'l', 'm', 'n',
                'o', 'p', 'q', 'r', 's', 't',
                'u', 'v', 'w', 'x', 'y', 'z'
        };
    private:
        constexpr static const int32_t sizeTable[] = {9, 99, 999, 9999, 99999, 999999, 9999999,
                                                      99999999, 999999999, Integer::MAX_VALUE};

        // Requires positive x
        static int32_t stringSize(int32_t x);

        static void getChars(int32_t i, int32_t index, char *buf);

    public:
        static String toString(int32_t i);

        /**
        *
        * @param s string
        * @param radix radix
        * @return result
        * @throw NumberFormatException
        */
        static int32_t javaParseInt(const String &s, int32_t radix);

        static int32_t javaParseInt(const String &s);;

        static int32_t javaParseUnsignedInt(const String &s, int32_t radix);

        static int32_t javaParseUnsignedInt(const String &s);

        static int32_t parseInt(const String &s, int32_t radix);

        static int32_t parseInt(const String &s);
    };

    class Long {
    private:
        // Requires positive x
        static int32_t stringSize(int64_t x);

        static void getChars(int64_t i, int32_t index, char *buf);

    public:
        static const int64_t MIN_VALUE = 0x8000000000000000L;
        static const int64_t MAX_VALUE = 0x7fffffffffffffffL;

        static String toString(int64_t i);

        /**
        *
        * @param s string
        * @param radix radix
        * @return result
        * @throw NumberFormatException
        */
        static int64_t javaParseLong(const String &s, int32_t radix);

        static int64_t javaParseLong(const String &s);

        /**
        *
        * @param s s
        * @param beginIndex i
        * @param endIndex i
        * @param radix radix
        * @return r
        * @throw NullPointerException
        * @throw IndexOutOfBoundsException
        * @throw NumberFormatException
        */
        static int64_t javaParseLong(const String &s, int32_t beginIndex, int32_t endIndex, int32_t radix);

        /**
        *
        * @param s string
        * @param radix radix
        * @return NumberFormatException
        */
        static int64_t javaParseUnsignedLong(const String &s, int32_t radix);

        static int64_t javaParseUnsignedLong(const String &s);

        static int64_t parseLong(const String &s, int32_t radix);

        static int64_t parseLong(const String &s);
    };

    class Float {
    public:
        static constexpr float NaN = NAN;
        static constexpr float POSITIVE_INFINITY = INFINITY;
        static constexpr float NEGATIVE_INFINITY = -INFINITY;

        static constexpr uint32_t SIGN_MASK = 0b10000000000000000000000000000000;
        static constexpr uint32_t EXP_MASK = 0b01111111100000000000000000000000;
        static constexpr uint32_t MANTISSA_MASK = 0b00000000011111111111111111111111;
        static constexpr uint32_t SIGN_SHIFT = 31;
        static constexpr uint32_t EXP_SHIFT = 23;
        static constexpr uint32_t MANTISSA_SHIFT = 0;

        //    private:
        static String toBinaryString(uint32_t a) {
            // 0b101010101010010101
            String r(32);
            bool start = false;
            for (int8_t i = 31; i >= 0; --i) {
                bool bit = (a >> ((uint8_t) i)) & 0b1U;
                if (bit) start = true;
                if (start) r.append(bit ? '1' : '0');
            }
            return r;
        }

    public:
        static String toString(float x) {
            uint32_t bits = *((uint32_t *) &x);
            bool isNegative = (bits & SIGN_MASK) >> SIGN_SHIFT == 1;
            uint32_t mantissa = (bits & MANTISSA_MASK) >> MANTISSA_SHIFT;
            int32_t exponent = ((bits & EXP_MASK) >> EXP_SHIFT) - 127;

            int32_t intPart;
            if (exponent <= 0) {
                intPart = 0;
            } else {
                intPart = (mantissa >> (uint32_t) (23 - exponent)) | (0b1U << (uint32_t) exponent);
            }
            String r = isNegative ? "-" : "";
            r += String::toString(intPart) += '.';
            return r;
        }
    };

    class Double {
    public:
        static constexpr float NaN = Float::NaN;
        static constexpr float POSITIVE_INFINITY = Float::POSITIVE_INFINITY;
        static constexpr float NEGATIVE_INFINITY = Float::NEGATIVE_INFINITY;
    };
}

#endif //CPP_UTILS_HPP
