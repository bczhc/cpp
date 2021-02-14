#ifndef BCZHC_UTF8_HPP
#define BCZHC_UTF8_HPP

#include "def.hpp"

namespace bczhc {
    struct SolvedUTF8Properties {
        int bytesLength;
        int codepoint;
    };

    int getUTF8BytesLength(uchar firstByte);

    void solveUTF8Bytes(SolvedUTF8Properties &solvedProperties, const uchar *bytes);

    int getUTF8Size(uint32_t codepoint);

    int unicode2UTF8(char *dest, uint32_t codepoint);

    uint32_t surrogateConvert(uint16_t lead, uint16_t trail);

    struct Surrogate {
        uint16_t lead;
        uint16_t trail;
    };

    Surrogate surrogateConvert(uint32_t codepoint);
} // namespace bczhc
#endif // BCZHC_UTF8_HPP
