//
// Created by root on 2020/4/2.
//

#ifndef CPP_EPICYCLE_H
#define CPP_EPICYCLE_H

#include "./ComplexValue.h"

class Epicycle {
public:
    _Float64 n{};
    ComplexValue c{};

    Epicycle();

    Epicycle(_Float64 n, const ComplexValue &c);
};

#endif //CPP_EPICYCLE_H
