//
// Created by root on 2020/4/2.
//

#ifndef CPP_COMPLEXINTEGRAL_H
#define CPP_COMPLEXINTEGRAL_H

#include "./ComplexValue.h"

namespace bczhc {
    class ComplexIntegral {
    public:
        int n = 100000;

        //梯形法求定积分
        ComplexValue getDefiniteIntegralByTrapezium(
                double x0, double xn,
                ComplexFunctionInterface &complexFunctionInterface);
    };
}// namespace bczhc

#endif// CPP_COMPLEXINTEGRAL_H
