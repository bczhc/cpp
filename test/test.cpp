#include <iostream>
#include <functional>
#include <sys/time.h>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"
#include "../String.h"
#include "../Concurrent.h"
#include "thread"
#include "../Sqlite3.h"


using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

int main() {
    typedef int(*fp)(int);
    fp p;
    {
        p = [](int i) {
            return i + 10;
        };
    }
    cout << p(24) << endl;

    float f = 1234.56789;
    cout << (int) f << endl;
    return 0;
}