//
// Created by root on 2020/4/2.
//

#ifndef CPP_COMPLEX_INTEGRAL_H
#define CPP_COMPLEX_INTEGRAL_H

#include "FourierSeries.h"

namespace bczhc {
    class ComplexIntegral {
    public:
        int integralN = 100000;

        //梯形法求定积分
        ComplexValue getDefiniteIntegralByTrapezium(double x0, double xn,
                                                    bczhc::FuncInIntegralInterface &complexFunctionInterface,
                                                    int32_t n) const;
    };
}// namespace bczhc

#endif// CPP_COMPLEX_INTEGRAL_H
