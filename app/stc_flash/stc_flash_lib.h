//
// Created by zhc on 1/31/21.
//

#ifndef CPP_STC_FLASH_LIB_H
#define CPP_STC_FLASH_LIB_H

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

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;
using namespace bczhc::array;
using namespace concurrent;
using namespace symboltable;

using uchar = unsigned char;

namespace bczhc {
    class EchoCallback {
    public:
        virtual void print(const char *s) = 0;

        virtual void flush() = 0;
    };

    int run(const String &hexFile, EchoCallback *echoCallback);

    timespec timespec_from_ms(uint32_t millis);

    bool waitReadable(int fd, uint32_t timeout);

    class Serial {
    public:
        static inline char PARITY_NONE = 'N';
        static inline char PARITY_EVEN = 'E';
        static inline char PARITY_ODD = 'O';
        static inline char PARITY_MARK = 'M';
        static inline char PARITY_SPACE = 'S';

        virtual void setSpeed(unsigned int speed) = 0;

        virtual void close() = 0;

        [[nodiscard]] virtual Array<uchar> read(ssize_t size) = 0;

        virtual ssize_t write(uchar *buf, ssize_t size) = 0;

        [[nodiscard]] virtual unsigned int getBaud() = 0;

        virtual void flush() = 0;

        virtual void setTimeout(uint32_t t) = 0;

        virtual void setParity(char p) = 0;

        [[nodiscard]] virtual char getParity() = 0;

        [[nodiscard]] virtual uint32_t getTimeout() = 0;
    };
}

String ucharArr2String(const Array<uchar> &arr);


#endif //CPP_STC_FLASH_LIB_H
