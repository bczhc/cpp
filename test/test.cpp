#include <iostream>
#include <functional>
#include <string>
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

String int2bin(int a) {

}

String float2bin(float f) {
    String s;
    float t = f;
    for (int i = 0;; ++i) {
        t = 2 * (t >= 1 ? (t - 1) : t);
        if (t == 0) break;
        s.append(t >= 1 ? '1' : '0');
    }
    return s;
}

String double2bin(double f) {
    String s;
    double t = f;
    for (int i = 0;; ++i) {
        t = 2 * (t >= 1 ? (t - 1) : t);
        if (t == 0) break;
        s.append(t >= 1 ? '1' : '0');
    }
    return s;
}

int main() {
    float f = 263.3;
    char *p = (char *) &f;
    cout << hex << (int) (unsigned char) p[0] << ' ';
    cout << hex << (int) (unsigned char) p[1] << ' ';
    cout << hex << (int) (unsigned char) p[2] << ' ';
    cout << hex << (int) (unsigned char) p[3] << ' ' << endl;

    String s = "abcde";
    s.insert(1, "fg");
    cout << s.getCString() << endl;

    String ss = "ab";
    for (int i = 0; i < 20; ++i) {
        ss.append("12");
    }

    cout << ss.getCppString() << endl;

    return 0;
}
