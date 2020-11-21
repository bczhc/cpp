//
// Created by zhc on 11/21/20.
//

#include "Base128Lib.h"

void bczhc::base128::encode7bytes(char *dest, const char *src) {
    dest[0] = (char) ((src[0] & 255) >> 1);
    dest[1] = (char) (((src[0] & 1) << 6) | ((src[1] & 255) >> 2));
    dest[2] = (char) (((src[1] & 3) << 5) | ((src[2] & 255) >> 3));
    dest[3] = (char) (((src[2] & 7) << 4) | ((src[3] & 255) >> 4));
    dest[4] = (char) (((src[3] & 15) << 3) | ((src[4] & 255) >> 5));
    dest[5] = (char) (((src[4] & 31) << 2) | ((src[5] & 255) >> 6));
    dest[6] = (char) (((src[5] & 63) << 1) | ((src[6] & 255) >> 7));
    dest[7] = (char) (src[6] & 127);
}

void bczhc::base128::decode8bytes(char *dest, const char *src) {
    dest[0] = (char) (((src[0] & 255) << 1) | ((src[1] & 255) >> 6));
    dest[1] = (char) (((src[1] & 255) << 2) | ((src[2] & 255) >> 5));
    dest[2] = (char) (((src[2] & 255) << 3) | ((src[3] & 255) >> 4));
    dest[3] = (char) (((src[3] & 255) << 4) | ((src[4] & 255) >> 3));
    dest[4] = (char) (((src[4] & 255) << 5) | ((src[5] & 255) >> 2));
    dest[5] = (char) (((src[5] & 255) << 6) | ((src[6] & 255) >> 1));
    dest[6] = (char) (((src[6] & 255) << 7) | (src[7] & 255));
}
