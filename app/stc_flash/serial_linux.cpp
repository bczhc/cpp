//
// Created by zhc on 1/31/21.
//

#include "serial_linux.h"

timespec bczhc::timespec_from_ms(const uint32_t millis) {
    timespec time{};
    time.tv_sec = millis / 1e3;
    time.tv_nsec = (millis - (time.tv_sec * 1e3)) * 1e6;
    return time;
}


bool bczhc::waitReadable(int fd, uint32_t timeout) {
    // Setup a select call to block for serial data or a timeout
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    timespec timeout_ts(timespec_from_ms(timeout));
    int r = pselect(fd + 1, &readfds, nullptr, nullptr, &timeout_ts, nullptr);

    if (r < 0) {
        // Select was interrupted
        if (errno == EINTR) {
            return false;
        }
        // Otherwise there was some error
        throw String("errno: ") + String::toString(errno);
    }
    // Timeout occurred
    if (r == 0) {
        return false;
    }
    // This shouldn't happen, if r > 0 our fd has to be in the list!
    if (!FD_ISSET (fd, &readfds)) {
        throw String("select reports ready to read, but our fd isn't"
                     " in the list, this shouldn't happen!");
    }
    // Data available to read.
    return true;
}

void setFlags(int fd) {
    termios options{};
    tcgetattr(fd, &options);
    // set up raw mode / no echo / binary
    options.c_cflag |= (tcflag_t) (CLOCAL | CREAD);
    options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
                                    ISIG | IEXTEN); //|ECHOPRT

    options.c_oflag &= (tcflag_t) ~(OPOST);
    options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);
#ifdef IUCLC
    options.c_iflag &= (tcflag_t) ~IUCLC;
#endif
#ifdef PARMRK
    options.c_iflag &= (tcflag_t) ~PARMRK;
#endif
    tcsetattr(fd, TCSANOW, &options);
}


SerialLinux::SerialLinux(int fd, uint32_t baud, uint32_t timeout) : fd(fd), baud(baud), timeout(timeout) {
    setSpeed(baud), setTimeout(timeout);
}

SerialLinux::SerialLinux(const char *port, uint32_t baud, uint32_t timeout) {
    int o = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (o == -1) throw String("Open port failed.");
    this->fd = o;
    setSpeed(baud);
    setTimeout(timeout);
}

void SerialLinux::setSpeed(unsigned int speed) {
    unsigned int speedArr[] = {B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600,
                               B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600,
                               B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000};
    unsigned int nameArr[] = {0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400,
                              57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000,
                              2000000, 2500000, 3000000, 3500000, 4000000};
    termios opt{};
    tcgetattr(this->fd, &opt);
    int len = ARR_SIZE(speedArr);
    for (int i = 0; i < len; i++) {
        if (speed == nameArr[i]) {
            tcflush(this->fd, TCIOFLUSH);
            unsigned int rate = speedArr[i];
            cfsetispeed(&opt, rate);
            cfsetospeed(&opt, rate);
            int status = tcsetattr(this->fd, TCSANOW, &opt);
            if (status != 0) throw String("tcsetattr fd1");
            this->baud = speed;
            return;
        }
        tcflush(this->fd, TCIOFLUSH);
    }
    throw String("Setting speed failed.");
}

void SerialLinux::close() {
    if (::close(this->fd) != 0) throw String("Close failed.");
}

Array<uchar> SerialLinux::read(ssize_t size) {
    int64_t start = getCurrentTimeMillis();
    char buf[size];
    ssize_t haveRead = 0;
    while (true) {
        bool b = waitReadable(this->fd, this->timeout);
        if (b) {
            if (getCurrentTimeMillis() - start >= this->timeout) {
                // timeout occurred
                break;
            }
            haveRead += ::read(this->fd, buf + haveRead, size - haveRead);
            if (haveRead == size) {
                // enough data
                break;
            }
        } else {
            // timeout occurred
            break;
        }
    }
    Array<uchar> r(haveRead);
    for (int i = 0; i < haveRead; ++i) {
        r[i] = buf[i];
    }
    return r;
}

ssize_t SerialLinux::write(uchar *buf, ssize_t size) {
    ssize_t i = ::write(this->fd, buf, size);
    if (i == -1) throw String("write error");
    return i;
}

unsigned int SerialLinux::getBaud() {
    return this->baud;
}

void SerialLinux::flush() const {
    // `::write()` has no cache, so there's no need to "flush".
}

void SerialLinux::setTimeout(uint32_t t) {
    this->timeout = t;
}

void SerialLinux::setParity(char p) {
    this->parity = p;
}

char SerialLinux::getParity() {
    return this->parity;
}

uint32_t SerialLinux::getTimeout() {
    return this->timeout;
}

void SerialLinux::flush() {
    // empty implementation
}
