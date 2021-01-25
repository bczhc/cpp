#include "../concurrent.h"
#include "../file.h"
#include "../io.h"
#include "../math/fourier_series.h"
#include "../regex.h"
#include "../sqlite3.h"
#include "../string.hpp"
#include "../third_party/practice/LinearList.hpp"
#include "../utils.hpp"
#include "thread"
#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <ifaddrs.h>
#include <iostream>
#include <netinet/in.h>
#include <new>
#include <pcre.h>
#include <pthread.h>
#include <string>
#include "../array.hpp"
#include <fcntl.h>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;
using namespace bczhc::array;

MutexLock lock;         // NOLINT(cert-err58-cpp)
CountDownLatch latch(2);// NOLINT(cert-err58-cpp)

int main(int argc, char **argv) {
    using fd = int;
    fd f = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NONBLOCK);
    char c;
    while (read(f, &c, 1) > 0) {
        cout << c;
    }
    cout << endl;
    return 0;
}
