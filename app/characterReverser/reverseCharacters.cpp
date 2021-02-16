#include "reverseCharacters.h"
#include "../../utf8.hpp"

inline void cpyStr(char *dest, char *s, int length) {
    for (int i = 0; i < length; ++i)
        dest[i] = s[i];
}

void bczhc::reverseCharacters(char *dest, char *s, int length) {
    int i = 0, destTail = length, bytesLength;
    for (;;) {
        bytesLength = getUTF8BytesLength(s[i]);
        destTail -= bytesLength;
        cpyStr(dest + destTail, s + i, bytesLength);
        i += bytesLength;
        if (i == length)
            break;
    }
}
