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

int main() {
    std::string ss = "不是a2";
    cout << ss.size() << endl;
    cout << ss.length() << endl;
    timeval start, t;
    gettimeofday(&start, nullptr);
    String s = "ab";
    //std::string s = "ab";
    for (int i = 0; i < 25; ++i) {
        s.append(s);
    }
    gettimeofday(&t, nullptr);
    cout << (t.tv_sec * 1000 - start.tv_sec * 1000 + t.tv_usec / 1000 - start.tv_usec / 1000) << endl;
    return 0;
}
