//
// Created by root on 2020/3/27.
//

#include "../zhc.h"
#include "../FourierSeries.h"
#include <iostream>
#include <bits/stdint-intn.h>
#include <cstdio>
#include <iostream>

void fff(int);

int main() {
    double T = 50, omega = 2 * M_PI / T;
    class F : public ComplexFunctionInterface {
    public:
        F(double mT, double mOmega) : mT(mT), mOmega(mOmega) {}

        double mT, mOmega, n;

        void x(ComplexValue &dest, double t) override {
            dest.setValue(10, 10);
            dest.selfMultiply(cos(-n * mOmega * t), sin(-n * mOmega * t));
        }
    };
    F f(T, omega);
    f.n = 1;

    ComplexIntegral integral{.n = 10000};
    ComplexValue r = integral.getDefiniteIntegralByTrapezium(0, T, f).selfDivide(T, 0);
    cout << r.toString() << endl;
    FourierSeries fs(f, 3, 3);
    class CB : public FourierSeriesCallback {

    } cb;
    fs.calc(cb, 5, 5);
}
