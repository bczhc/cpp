//
// Created by zhc on 11/21/20.
//

#include "Base128Lib.h"

void bczhc::base128::encode7bytes(uchar *dest, const uchar *src) {
    dest[0] = ((src[0] & 255U) >> 1U);
    dest[1] = (((src[0] & 1U) << 6U) | ((src[1] & 255U) >> 2U));
    dest[2] = (((src[1] & 3U) << 5U) | ((src[2] & 255U) >> 3U));
    dest[3] = (((src[2] & 7U) << 4U) | ((src[3] & 255U) >> 4U));
    dest[4] = (((src[3] & 15U) << 3U) | ((src[4] & 255U) >> 5U));
    dest[5] = (((src[4] & 31U) << 2U) | ((src[5] & 255U) >> 6U));
    dest[6] = (((src[5] & 63U) << 1U) | ((src[6] & 255U) >> 7U));
    dest[7] = (src[6] & 127U);
}

void bczhc::base128::decode8bytes(uchar *dest, const uchar *src) {
    dest[0] = (((src[0] & 255U) << 1U) | ((src[1] & 255U) >> 6U));
    dest[1] = (((src[1] & 255U) << 2U) | ((src[2] & 255U) >> 5U));
    dest[2] = (((src[2] & 255U) << 3U) | ((src[3] & 255U) >> 4U));
    dest[3] = (((src[3] & 255U) << 4U) | ((src[4] & 255U) >> 3U));
    dest[4] = (((src[4] & 255U) << 5U) | ((src[5] & 255U) >> 2U));
    dest[5] = (((src[5] & 255U) << 6U) | ((src[6] & 255U) >> 1U));
    dest[6] = (((src[6] & 255U) << 7U) | (src[7] & 255U));
}
