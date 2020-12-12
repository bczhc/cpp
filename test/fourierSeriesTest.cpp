#include "../FourierSeries.h"

using namespace bczhc;
using namespace concurrent;

int main() {
    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(t, t * t);
        }
    } f;
    FourierSeries fs(f, 10, 10);
    class C : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout.precision(16);
            cout << n << ' ' << re << ' ' << im << endl;
        }
    } c;
    fs.calc(c, 100000, 5);
    return 0;
}
