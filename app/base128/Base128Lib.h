//
// Created by zhc on 11/21/20.
//

#ifndef CPP_BASE128LIB_H
#define CPP_BASE128LIB_H

using uchar = unsigned char;

namespace bczhc::base128 {
    void encode7bytes(uchar *dest, const uchar src[7]);

    void decode8bytes(uchar *dest, const uchar src[8]);
}

#endif //CPP_BASE128LIB_H
