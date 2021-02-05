#include "../concurrent.h"
#include "../file.h"
#include "../io.h"
#include "../math/fourier_series.h"
#include "../regex.h"
#include "../sqlite3.h"
#include "../string.hpp"
#include "../third_party/practice/LinearList.hpp"
#include "../third_party/practice/SymbolTable.hpp"
#include "../utils.hpp"
#include "thread"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#ifdef __WIN32
#include <windows.h>
#endif
#ifdef CMAKE_PCRE_FOUND
#include <pcre.h>
#endif
#include "../array.hpp"
#include <cassert>
#include <cstdarg>
#include <fcntl.h>
#include <pthread.h>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace bczhc::array;
using namespace symboltable;

int main() {
    class CB : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout << String::toString(n).getCString() << ' ' << String::toString(re).getCString() << ' ' << String::toString(im).getCString() << endl;
        }
    } cb;

    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.setValue(sin(t), cos(t) * sin(t));
        }
    } f;

    FourierSeries fd(f, 100, 100);
    fd.calc(cb, 100000, 10);
    return 0;
}
