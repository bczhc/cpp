//
// Created by zhc on 1/31/21.
//

#ifndef BCZHC_STC_FLASH_LIB_H
#define BCZHC_STC_FLASH_LIB_H

#include <termios.h>
#include "../../string.hpp"
#include "../../file.hpp"
#include "../../io.hpp"
#include "../../utils.hpp"
#include "../../array.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "../../concurrent.hpp"
#include <cstdarg>
#include <cerrno>
#include "../../symbol_table.hpp"
#include <cassert>
#include <cmath>
#include "serial.h"

using namespace bczhc;

namespace bczhc {
    class EchoCallback {
    public:
        virtual void print(const char *s) = 0;

        virtual void flush() = 0;
    };

    int run(const String &hexFile, EchoCallback *echoCallback, Serial *serialImpl);
}

#endif //BCZHC_STC_FLASH_LIB_H
