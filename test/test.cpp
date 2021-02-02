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
#include <pcre.h>
#include <pthread.h>
#include "../array.hpp"
#include <fcntl.h>
#include <cstdarg>
#include <cassert>
#include "../app/stc_flash/stc_flash_lib.h"
#include <fcntl.h>
#include "../app/stc_flash/serial_linux.h"
#include "serial.h"
#include "serial_linux.h"

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

int main() {
    const serial::Serial &s = SerialLinux("/dev/ttyUSB0", 2400, 50);
    uchar b[] = {0x7f, 0x7f};
    int i;
    for (i = 0; i < 1000; ++i) {
        s.write(b, 2);
        auto r = s.read(10);
        cout << "read: " << r.toString().getCString() << endl;
    }
    if (i == 1000) throw String("out of waiting time");
    return 0;
}