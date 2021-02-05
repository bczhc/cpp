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
    String s = "abc";
    cout << s.utf8Length() << endl;
    return 0;
}