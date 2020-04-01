//
// Created by root on 2020/3/31.
//

#include "./ComplexValue.h"
#include "./ComplexDefinite.h"

class FourierSeries {
private:
    int32_t omega{};
    int32_t period{};
     OneVariableComplexFunction_t f{};
public:
    explicit FourierSeries(OneVariableComplexFunction_t pFunction, int32_t period) {
        f = pFunction;
        omega = M_PIf64 * 2 / period;
    }
};

double f(double x) {
    return x * x;
}

int main() {
    return 0;
}