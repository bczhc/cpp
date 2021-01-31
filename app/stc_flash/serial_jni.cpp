//
// Created by zhc on 1/31/21.
//

#include "stc_flash_lib.h"

Serial::Serial(int, uint32_t baud, uint32_t timeout) {

}

Serial Serial::open(const char *, uint32_t timeout) {
    return Serial(0, 9600, timeout);
}

void Serial::setSpeed(unsigned int speed) {

}

void Serial::close() {

}

Array<uchar> Serial::read(ssize_t size) {
    return Array<uchar>(0);
}

ssize_t Serial::write(uchar *buf, ssize_t size) {
    return 0;
}

unsigned int Serial::getBaud() {
    return 0;
}

void Serial::flush() const {

}

void Serial::setTimeout(uint32_t t) {

}

void Serial::setParity(char p) {

}

char Serial::getParity() {
    return 0;
}