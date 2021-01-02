//
// Created by bczhc on 2020/3/25.
//
#include "String.h"

using namespace bczhc::string;

int utf8::getUTF8Size(int codepoint) {
    if (codepoint >= 0x0 && codepoint <= 0x7f) {
        return 1;
    } else if (codepoint >= 0x80 && codepoint <= 0x7ff) {
        return 2;
    } else if (codepoint >= 0x800 && codepoint <= 0xffff) {
        return 3;
    } else if (codepoint >= 0x10000 && codepoint <= 0x10ffff) {
        return 4;
    }
    return 0;
}

int utf8::unicode2UTF8(char *dest, int codepoint) {
    int utf8Size = utf8::getUTF8Size(codepoint);
    switch (utf8Size) {
        case 1:
            dest[0] = codepoint & 0b01111111;
            break;
        case 2:
            dest[1] = 0b10000000 | (codepoint & 0b00111111);
            dest[0] = 0b11000000 | ((codepoint >> 6) & 0b00111111);
            break;
        case 3:
            dest[2] = 0b10000000 | (codepoint & 0b00111111);
            dest[1] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
            dest[0] = 0b11100000 | ((codepoint >> 12) & 0b00111111);
            break;
        case 4:
            dest[3] = 0b10000000 | (codepoint & 0b00111111);
            dest[2] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
            dest[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
            dest[0] = 0b11110000 | ((codepoint >> 18) & 0b00111111);
            break;
        default:
            break;
    }
    return utf8Size;
}

int utf8::getUTF8BytesLength(const unsigned char firstByte) {
    if (firstByte >> 7 == 0)
        return 1;
    if (firstByte >> 5 == 0b110)
        return 2;
    if (firstByte >> 4 == 0b1110)
        return 3;
    if (firstByte >> 3 == 0b11110)
        return 4;
    return 0;
}

void utf8::solveUTF8Bytes(utf8::SolvedUTF8Properties &solvedProperties,
                          const char *bytes) {
    solvedProperties.bytesLength = utf8::getUTF8BytesLength(bytes[0]);
    switch (solvedProperties.bytesLength) {
        case 1:
            solvedProperties.codepoint = bytes[0] & 0b01111111;
            break;
        case 2:
            solvedProperties.codepoint =
                    (bytes[1] & 0b00111111) | ((bytes[0] & 0b00011111) << 6);
            break;
        case 3:
            solvedProperties.codepoint = (bytes[2] & 0b00111111) |
                                         ((bytes[1] & 0b00111111) << 6) |
                                         ((bytes[0] & 0b00001111) << 12);
            break;
        case 4:
            solvedProperties.codepoint =
                    (bytes[3] & 0b00111111) | ((bytes[2] & 0b00111111) << 6) |
                    ((bytes[1] & 0b00111111) << 12) | ((bytes[0] & 0b00000111) << 18);
            break;
        default:
            break;
    }
}
