#include <iostream>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"
#include "../String.h"
#include "../Concurrent.h"


using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

void f(const String& s) {
    cout << s.getCString() << endl;
}

int main() {
    String s = "12345678";
    f(s);
    Thread::sleep(1000);
    cout << s.getCString() << endl;
    return 0;
    class CB : public FourierSeriesCallback {
    private:
        void f(String a, String s) {
            cout << a.getCString() << ' ' << s.getCString() << endl;
        }

    public:
        void callback(double n, double re, double im) override {
            String s = to_string(n).c_str();
            s.append(" ")
                    .append(to_string(re))
                    .append(" ")
                    .append(to_string(im));
            f("jni---", s);
        }
    } cb;

    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(sin(t + cos(t)), cos(t - 2));
        }
    } f;

    FourierSeries fs(f, 1000000, 100);
    fs.calc(cb, 10000, 8);
    return 0;
}