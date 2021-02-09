//
// Created by zhc on 2/1/21.
//

#ifndef BCZHC_SERIAL_SERIAL_LINUX_H
#define BCZHC_SERIAL_SERIAL_LINUX_H

#include "serial.h"

namespace bczhc {
    class SerialLinux : public Serial {
    private:
        int fd{};
        uint32_t baud{};
        uint32_t timeout{};
        char parity = PARITY_NONE;
        String portName = nullptr;

        static timespec timespec_from_ms(uint32_t millis);

        [[nodiscard]] bool waitReadable(uint32_t timeoutMillis) const;

        void setFlags() const;

    public:
        SerialLinux(int fd, uint32_t baud, uint32_t timeout);

        explicit SerialLinux(const char *port, uint32_t baud = 9600, uint32_t timeout = -1);

        void setSpeed(unsigned int speed) override;

        void close() const override;

        [[nodiscard]] Array<uchar> read(ssize_t size) const override;

        ssize_t write(uchar *buf, ssize_t size) const override;

        [[nodiscard]] unsigned int getBaud() const override;

        void flush() const override;

        void setTimeout(uint32_t t) override;

        void setParity(char p) override;

        [[nodiscard]] char getParity() const override;

        [[nodiscard]] uint32_t getTimeout() const override;

        [[nodiscard]] String getPortName() const override;
    };
}

#endif //BCZHC_SERIAL_SERIAL_LINUX_H
