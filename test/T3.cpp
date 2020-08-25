//
// Created by root on 2020/3/28.
//

#include "../FourierSeries.h"
#include <iostream>

using namespace std;

int main() {
    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(10, 10);
        }
    } f;
    FourierSeries fs(f, 2, 50);
    class CB : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout << "n: " << n << " re: " << re << " im: " << im << endl;
        }
    } cb;
    fs.calc(cb, 1000, 1);
}
