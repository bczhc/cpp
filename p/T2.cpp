//
// Created by root on 2020/3/27.
//

#include "../zhc.h"
#include "../FourierSeries.h"
#include <iostream>
using namespace std;

int main() {
    class Callback : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout << "n: " << n << " re: " << re << " im: " << im << endl;
        }
    } callback;

    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(10, 10);
        }
    } f;
    FourierSeries fs(f, 100, 1);
    fs.calc(callback);
}