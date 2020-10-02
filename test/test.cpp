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

MutexLock lock;

int main() {
    class CB : public FourierSeriesCallback {
    private:
        void f(String a, String s) {
            lock.lock();
            cout << a.getCString() << ' ' << s.getCString() << endl;
            lock.unlock();
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
            dest.setValue(10, 10 );
        }
    } f;

    FourierSeries fs(f, 100, 100);
    fs.calc(cb, 12345, 5);
    return 0;
}