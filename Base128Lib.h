//
// Created by zhc on 11/21/20.
//

#ifndef CPP_BASE128LIB_H
#define CPP_BASE128LIB_H

namespace bczhc::base128 {
    void encode7bytes(char *dest, const char src[7]);


    void decode8bytes(char *dest, const char src[8]);
}

#endif //CPP_BASE128LIB_H
