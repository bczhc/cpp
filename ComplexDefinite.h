//
// Created by root on 2020/4/2.
//

#ifndef CPP_COMPLEXDEFINITE_H
#define CPP_COMPLEXDEFINITE_H

#include "./ComplexValue.h"

class ComplexDefinite {
public:
    int n = 100000;

    //梯形法求定积分
    ComplexValue
    getDefiniteIntegralByTrapezium(_Float64 x0, _Float64 xn, OneVariableComplexFunction_t complexFunctionInterface);
};

#endif //CPP_COMPLEXDEFINITE_H
