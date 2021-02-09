//
// Created by root on 2020/4/2.
//

#ifndef BCZHC_EPICYCLE_H
#define BCZHC_EPICYCLE_H

#include "./complex_value.h"

namespace bczhc {

    class Epicycle {
    public:
        double n{};
        ComplexValue c{};

        Epicycle();

        Epicycle(double n, const ComplexValue &c);
    };

}// namespace bczhc

#endif// BCZHC_EPICYCLE_H
