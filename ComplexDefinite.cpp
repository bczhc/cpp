//
// Created by root on 2020/4/2.
//

#include "ComplexDefinite.h"

ComplexValue ComplexDefinite::getDefiniteIntegralByTrapezium(_Float64 x0, _Float64 xn,
                                                             OneVariableComplexFunction_t complexFunctionInterface) {
    _Float64 d = (xn - x0) / n;
    ComplexValue sum(0, 0);
    ComplexValue cv1(0, 0);
    ComplexValue cv2(2, 0);
    ComplexValue dComplex(d, 0);
    ComplexValue v{}, v2{};
    for (_Float64 i = x0; i <= xn; i += d) { // NOLINT(cert-flp30-c)
        complexFunctionInterface(v, i);
        complexFunctionInterface(v2, i + d);
        sum.selfAdd(cv1.setValue(v).add(v2).multiply(dComplex).selfDivide(cv2));
    }
    return sum;
}
