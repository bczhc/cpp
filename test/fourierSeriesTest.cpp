#include "../FourierSeries.h"

using namespace bczhc;

void f(int &i) {
    cout << i << endl;
}

int main() {
    int b = 3;
    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(t, t * t);
        }
    } f;
    FourierSeries fs(f, 500, 10);
    class C : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout.precision(16);
            cout << n << ' ' << re << ' ' << im << endl;
        }
    } c;
    fs.calc(c, 100000, 3);
    return 0;
}
