//
// Created by root on 2020/4/2.
//

#include "ComplexDefinite.h"

ComplexValue ComplexDefinite::getDefiniteIntegralByTrapezium(double x0, double xn,
                                                             ComplexFunctionInterface &complexFunctionInterface) {
    double d = (xn - x0) / n;
    ComplexValue sum(0, 0);
    ComplexValue cv1(0, 0);
    ComplexValue cv2(2, 0);
    ComplexValue dComplex(d, 0);
    ComplexValue v{}, v2{};
    for (double i = x0; i <= xn; i += d) { // NOLINT(cert-flp30-c)
        complexFunctionInterface.x(v, i);
        complexFunctionInterface.x(v2, i + d);
        sum.selfAdd(cv1.setValue(v).add(v2).multiply(dComplex).selfDivide(cv2));
    }
    return sum;
}
