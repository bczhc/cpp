#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include <sys/time.h>


using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

int main() {
    String s(100000 * 5);
    s.append("afds");
    s.append("afds");
    s.append("afds");
    s.append("afds");
    s.clear();
    for (int i = 0; i < 100000; ++i) {
        s.append("12345");
    }
    return 0;
}
