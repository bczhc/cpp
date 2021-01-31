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
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    termios options{};
    tcgetattr(fd, &options);
    options.c_cflag |= (tcflag_t) (CLOCAL | CREAD);
    options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
                                    IEXTEN); //|ECHOPRT

    options.c_oflag &= (tcflag_t) ~(OPOST);
    options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);
#ifdef IUCLC
    options.c_iflag &= (tcflag_t) ~IUCLC;
#endif
#ifdef PARMRK
    options.c_iflag &= (tcflag_t) ~PARMRK;
#endif
    tcsetattr(fd, TCSANOW, &options);

    cout << "hello\n";
    cout << "hello... ";
    char a[] = {1, 2, 3, 4, 5};
    write(fd, a, 5);
    cout << "done\n";
    cout << "hello\n";

    close(fd);
    return 0;
}