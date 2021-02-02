//
// Created by zhc on 1/31/21.
//

#ifndef BCZHC_STC_FLASH_LIB_H
#define BCZHC_STC_FLASH_LIB_H

#include <termios.h>
#include "../../string.hpp"
#include "../../file.h"
#include "../../io.h"
#include "../../utils.hpp"
#include "../../array.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "../../concurrent.h"
#include <cstdarg>
#include <cerrno>
#include "../../third_party/practice/SymbolTable.hpp"
#include <cassert>
#include <cmath>
#include "serial.h"

using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;
using namespace bczhc::array;
using namespace concurrent;
using namespace symboltable;
using namespace bczhc::serial;

namespace bczhc {
    class EchoCallback {
    public:
        virtual void print(const char *s) = 0;

        virtual void flush() = 0;
    };

    int run(const String &hexFile, EchoCallback *echoCallback, Serial *serialImpl);
}

String ucharArr2String(const Array<uchar> &arr);


#endif //BCZHC_STC_FLASH_LIB_H
