#ifndef BCZHC_UTF8_H
#define BCZHC_UTF8_H

#include "def.h"

namespace bczhc {
    struct SolvedUTF8Properties {
        int bytesLength;
        int codepoint;
    };

    int getUTF8BytesLength(uchar firstByte);

    void solveUTF8Bytes(SolvedUTF8Properties &solvedProperties, const uchar *bytes);

    int getUTF8Size(uint32_t codepoint);

    int unicode2UTF8(char *dest, uint32_t codepoint);
} // namespace bczhc
#endif // BCZHC_UTF8_H
