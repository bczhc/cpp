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
#include <cstdarg>

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
using namespace symboltable;

MutexLock lock;         // NOLINT(cert-err58-cpp)
CountDownLatch latch(2);// NOLINT(cert-err58-cpp)

using uchar = uint8_t;

template<typename T>
using Tuple = Array<T>;

template<typename ArgType, typename ReturnType>
Tuple<ReturnType> f(int size, ArgType a...) {
    Tuple<ReturnType> r(size);
    r[0] = a;
    va_list args{};
    va_start(args, a);
    for (int i = 1; i < size; ++i) {
        r[i] = va_arg(args, ArgType);
    }
    va_end(args);
    return r;
}


int main(int argc, char **argv) {
    return 0;
}
