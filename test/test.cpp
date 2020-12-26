#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include "../utils.hpp"
#include "../File.h"
#include "../io.h"
#include <pcre.h>
#include "../RegExp.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;

MutexLock lock; // NOLINT(cert-err58-cpp)
CountDownLatch latch(2); // NOLINT(cert-err58-cpp)


int main(int argc, char **argv) {
    auto matched = match("(?<=Cmake version: )[0-9]\\.[0-9]\\.[0-9](\\.[0-9])?", "Cmake version: 1.2.3.1");
    auto it = matched.getIterator();
    if (it.moveToFirst()) {
        do {
            cout << it.get().getCString() << endl;
        } while (it.next());
    }

    cout << test("a...e", "abcde") << endl;
    cout << test("a...e.", "abcde") << endl;
    return 0;
}
