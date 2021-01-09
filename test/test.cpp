#include "../Concurrent.h"
#include "../File.h"
#include "../IO.h"
#include "../RegExp.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../Utils.hpp"
#include "../math/FourierSeries.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <new>
#include <pcre.h>
#include <pthread.h>
#include <string>
#include <unistd.h>

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
    FILE *f = popen("ls", "r");
    if (f == nullptr) {
        cout << "popen error" << endl;
        return 1;
    }

    auto is = InputStream(f);
    LineReader reader = LineReader(is);
    String line;
    while (true) {
        line = reader.readLine();
        if (line.isNull()) break;
        cout << line.getCString() << endl;
    }
    is.close();
    return 0;
}
