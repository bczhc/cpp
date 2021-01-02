//
// Created by root on 2020/4/2.
//

#include "ComplexIntegral.h"

using namespace bczhc;

ComplexValue bczhc::ComplexIntegral::getDefiniteIntegralByTrapezium(double x0, double xn,
                                                                    FuncInIntegralInterface &complexFunctionInterface,
                                                                    int32_t n) const {
    double d = (xn - x0) / integralN;
    ComplexValue sum(0, 0), left(0, 0), right(0, 0);
    for (double t = 0; t <= xn; t += d) {// NOLINT(cert-flp30-c)
        complexFunctionInterface.x(left, t, n), complexFunctionInterface.x(right, t + d, n);
        sum.selfAdd(left.selfAdd(right).selfMultiply(d, 0).selfDivide(2, 0));
    }
    return sum;
}
