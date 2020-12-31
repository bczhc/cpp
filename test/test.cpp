#include "../Concurrent.h"
#include "../File.h"
#include "../FourierSeries.h"
#include "../RegExp.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../io.h"
#include "../third_party/practice/LinearList.hpp"
#include "../utils.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <pcre.h>
#include <string>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;

MutexLock lock;         // NOLINT(cert-err58-cpp)
CountDownLatch latch(2);// NOLINT(cert-err58-cpp)

int main(int argc, char **argv) {
    try {
        auto is = InputStream(stdin);

        auto lr = LineReader(is);

        for(;;) {
            auto s = lr.readLine();
            if (s.isNull()) break;
            cout << s.getCString() << endl;
        }
    } catch (String e) {
        cout << e.getCString() << endl;
    }
    return 0;
}
