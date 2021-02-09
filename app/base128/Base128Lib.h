//
// Created by zhc on 11/21/20.
//

#ifndef BCZHC_BASE128LIB_H
#define BCZHC_BASE128LIB_H

using uchar = unsigned char;

namespace bczhc {
    void encode7bytes(uchar *dest, const uchar src[7]);

    void decode8bytes(uchar *dest, const uchar src[8]);
}

#endif //BCZHC_BASE128LIB_H
