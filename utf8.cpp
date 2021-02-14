//
// Created by bczhc on 2020/3/25.
//
#include "utf8.hpp"

using namespace bczhc;

int bczhc::getUTF8Size(uint32_t codepoint) {
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

int bczhc::unicode2UTF8(char *dest, uint32_t codepoint) {
    int utf8Size = bczhc::getUTF8Size(codepoint);
    switch (utf8Size) {
        case 1:
            dest[0] = (uchar) (codepoint & 0b01111111U);
            break;
        case 2:
            dest[1] = (uchar) (0b10000000U | (codepoint & 0b00111111U));
            dest[0] = (uchar) (0b11000000U | ((codepoint >> 6U) & 0b00111111U));
            break;
        case 3:
            dest[2] = (uchar) (0b10000000U | (codepoint & 0b00111111U));
            dest[1] = (uchar) (0b10000000U | ((codepoint >> 6U) & 0b00111111U));
            dest[0] = (uchar) (0b11100000U | ((codepoint >> 12U) & 0b00111111U));
            break;
        case 4:
            dest[3] = (uchar) (0b10000000U | (codepoint & 0b00111111U));
            dest[2] = (uchar) (0b10000000U | ((codepoint >> 6U) & 0b00111111U));
            dest[1] = (uchar) (0b10000000U | ((codepoint >> 12U) & 0b00111111U));
            dest[0] = (uchar) (0b11110000U | ((codepoint >> 18U) & 0b00111111U));
            break;
        default:
            break;
    }
    return utf8Size;
}

int bczhc::getUTF8BytesLength(const uchar firstByte) {
    if (firstByte >> 7U == 0)
        return 1;
    if (firstByte >> 5U == 0b110)
        return 2;
    if (firstByte >> 4U == 0b1110)
        return 3;
    if (firstByte >> 3U == 0b11110)
        return 4;
    return 0;
}

void bczhc::solveUTF8Bytes(SolvedUTF8Properties &solvedProperties, const uchar *bytes) {
    solvedProperties.bytesLength = getUTF8BytesLength(bytes[0]);
    switch (solvedProperties.bytesLength) {
        case 1:
            solvedProperties.codepoint = (int) (bytes[0] & 0b01111111U);
            break;
        case 2:
            solvedProperties.codepoint = (int) ((bytes[1] & 0b00111111U)
                                                | ((bytes[0] & 0b00011111U) << 6U));
            break;
        case 3:
            solvedProperties.codepoint = (int) ((bytes[2] & 0b00111111U)
                                                | ((bytes[1] & 0b00111111U) << 6U)
                                                | ((bytes[0] & 0b00001111U) << 12U));
            break;
        case 4:
            solvedProperties.codepoint = (int) ((bytes[3] & 0b00111111U)
                                                | ((bytes[2] & 0b00111111U) << 6U)
                                                | ((bytes[1] & 0b00111111U) << 12U)
                                                | ((bytes[0] & 0b00000111U) << 18U));
            break;
        default:
            break;
    }
}

Surrogate bczhc::surrogateConvert(uint32_t codepoint) {
    constexpr uint32_t LEAD_OFFSET = 0xd800 - (0x10000U >> 10U);
    uint16_t lead = LEAD_OFFSET + (codepoint >> 10U);
    uint16_t trail = 0xDc00 + (codepoint & 0x3FFU);
    return Surrogate{.lead = lead, .trail = trail};
}

uint32_t bczhc::surrogateConvert(uint16_t lead, uint16_t trail) {
    constexpr uint32_t SURROGATE_OFFSET = 0x10000 - (0xD800U << 10U) - 0xDc00;
    return (lead << 10U) + trail + SURROGATE_OFFSET;
}

