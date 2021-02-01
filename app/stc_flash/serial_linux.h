//
// Created by zhc on 2/1/21.
//

#ifndef CPP_SERIAL_LINUX_H
#define CPP_SERIAL_LINUX_H

#include "stc_flash_lib.h"

class SerialLinux : public Serial {
private:
    int fd{};
    uint32_t baud{};
    uint32_t timeout{};
    char parity = Serial::PARITY_NONE;
public:
    SerialLinux(int fd, uint32_t baud, uint32_t timeout);

    explicit SerialLinux(const char *port, uint32_t baud = 9600, uint32_t timeout = -1);

    void setSpeed(unsigned int speed) override;

    void close() override;

    Array<uchar> read(ssize_t size) override;

    ssize_t write(uchar *buf, ssize_t size) override;

    unsigned int getBaud() override;

    void flush() const;

    void setTimeout(uint32_t t) override;

    void setParity(char p) override;

    char getParity() override;

    uint32_t getTimeout() override;

    void flush() override;
};

#endif //CPP_SERIAL_LINUX_H
