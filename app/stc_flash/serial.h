//
// Created by zhc on 2/2/21.
//

#ifndef BCZHC_SERIAL_SERIAL_H
#define BCZHC_SERIAL_SERIAL_H

#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include "../../array.hpp"
#include <termios.h>
#include <sys/select.h>
#include <cerrno>
#include "../../utils.hpp"

using uchar = uint8_t;
using namespace bczhc::array;

namespace bczhc::serial {
    class Serial {
    public:
        static inline char PARITY_NONE = 'N';
        static inline char PARITY_EVEN = 'E';
        static inline char PARITY_ODD = 'O';
        static inline char PARITY_MARK = 'M';
        static inline char PARITY_SPACE = 'S';

        virtual void setSpeed(unsigned int speed) = 0;

        virtual void close() const = 0;

        [[nodiscard]] virtual Array<uchar> read(ssize_t size) const = 0;

        virtual ssize_t write(uchar *buf, ssize_t size) const = 0;

        [[nodiscard]] virtual unsigned int getBaud() const= 0;

        virtual void flush() const= 0;

        virtual void setTimeout(uint32_t t) = 0;

        virtual void setParity(char p) = 0;

        [[nodiscard]] virtual char getParity() const= 0;

        [[nodiscard]] virtual uint32_t getTimeout() const= 0;
    };
}

#endif //BCZHC_SERIAL_SERIAL_H
