#ifndef UTF8_H
#define UTF8_H

namespace bczhc::utf8 {
        struct SolvedUTF8Properties {
            int bytesLength;
            int codepoint;
        };

        int getUTF8BytesLength(unsigned char firstByte);

        void solveUTF8Bytes(SolvedUTF8Properties &solvedProperties, const char *bytes);

        int getUTF8Size(int codepoint);

        int unicode2UTF8(char *dest, int codepoint);
    } // namespace bczhc
#endif // UTF8_H
