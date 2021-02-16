//
// Created by zhc on 1/31/21.
//

#include "serial_linux.h"

using namespace bczhc;

timespec SerialLinux::timespec_from_ms(const uint32_t millis) {
    timespec time{};
    time.tv_sec = millis / 1e3;
    time.tv_nsec = (millis - (time.tv_sec * 1e3)) * 1e6;
    return time;
}


bool SerialLinux::waitReadable(uint32_t timeoutMillis) const {
    // Setup a select call to block for serial data or a timeout
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    timespec timeout_ts(timespec_from_ms(timeoutMillis));
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
    if (!FD_ISSET(fd, &readfds)) {
        throw String("select reports ready to read, but our fd isn't"
                     " in the list, this shouldn't happen!");
    }
    // Data available to read.
    return true;
}

void SerialLinux::setFlags() const {
    termios options{};
    tcgetattr(fd, &options);
    // set up raw mode / no echo / binary
    options.c_cflag |= (tcflag_t)(CLOCAL | CREAD);
    options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
                                    ISIG | IEXTEN);//|ECHOPRT

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
    setFlags();
}

SerialLinux::SerialLinux(const char *port, uint32_t baud, uint32_t timeout) {
    this->portName = port;
    int o = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (o == -1) throw String("Open port failed.");
    this->fd = o;
    setSpeed(baud);
    setTimeout(timeout);
    setFlags();
}

void SerialLinux::setSpeed(unsigned int speed) {
    uint32_t rate = 0;
    switch (speed) {
#ifdef B0
        case 0:
            rate = B0;
            break;
#endif
#ifdef B50
        case 50:
            rate = B50;
            break;
#endif
#ifdef B75
        case 75:
            rate = B75;
            break;
#endif
#ifdef B110
        case 110:
            rate = B110;
            break;
#endif
#ifdef B134
        case 134:
            rate = B134;
            break;
#endif
#ifdef B150
        case 150:
            rate = B150;
            break;
#endif
#ifdef B200
        case 200:
            rate = B200;
            break;
#endif
#ifdef B300
        case 300:
            rate = B300;
            break;
#endif
#ifdef B600
        case 600:
            rate = B600;
            break;
#endif
#ifdef B1200
        case 1200:
            rate = B1200;
            break;
#endif
#ifdef B1800
        case 1800:
            rate = B1800;
            break;
#endif
#ifdef B2400
        case 2400:
            rate = B2400;
            break;
#endif
#ifdef B4800
        case 4800:
            rate = B4800;
            break;
#endif
#ifdef B7200
            case 7200:
            rate = B7200;
            break;
#endif
#ifdef B9600
        case 9600:
            rate = B9600;
            break;
#endif
#ifdef B14400
            case 14400:
            rate = B14400;
            break;
#endif
#ifdef B19200
        case 19200:
            rate = B19200;
            break;
#endif
#ifdef B28800
            case 28800:
            rate = B28800;
            break;
#endif
#ifdef B57600
        case 57600:
            rate = B57600;
            break;
#endif
#ifdef B76800
            case 76800:
            rate = B76800;
            break;
#endif
#ifdef B38400
        case 38400:
            rate = B38400;
            break;
#endif
#ifdef B115200
        case 115200:
            rate = B115200;
            break;
#endif
#ifdef B128000
            case 128000:
            rate = B128000;
            break;
#endif
#ifdef B153600
            case 153600:
            rate = B153600;
            break;
#endif
#ifdef B230400
        case 230400:
            rate = B230400;
            break;
#endif
#ifdef B256000
            case 256000:
            rate = B256000;
            break;
#endif
#ifdef B460800
        case 460800:
            rate = B460800;
            break;
#endif
#ifdef B500000
        case 500000:
            rate = B500000;
            break;
#endif
#ifdef B576000
        case 576000:
            rate = B576000;
            break;
#endif
#ifdef B921600
        case 921600:
            rate = B921600;
            break;
#endif
#ifdef B1000000
        case 1000000:
            rate = B1000000;
            break;
#endif
#ifdef B1152000
        case 1152000:
            rate = B1152000;
            break;
#endif
#ifdef B1500000
        case 1500000:
            rate = B1500000;
            break;
#endif
#ifdef B2000000
        case 2000000:
            rate = B2000000;
            break;
#endif
#ifdef B2500000
        case 2500000:
            rate = B2500000;
            break;
#endif
#ifdef B3000000
        case 3000000:
            rate = B3000000;
            break;
#endif
#ifdef B3500000
        case 3500000:
            rate = B3500000;
            break;
#endif
#ifdef B4000000
        case 4000000:
            rate = B4000000;
            break;
#endif
        default:
            break;
    }

    termios opt{};
    tcgetattr(this->fd, &opt);
    cfsetispeed(&opt, rate);
    cfsetospeed(&opt, rate);
    int status = tcsetattr(this->fd, TCSANOW, &opt);
    if (status != 0) throw String("tcsetattr fd1: Setting baud failed");
    this->baud = speed;
}

void SerialLinux::close() const {
    if (::close(this->fd) != 0) throw String("Close failed.");
}

Array<uchar> SerialLinux::read(ssize_t size) const {
    int64_t start = getCurrentTimeMillis();
    char buf[size];
    ssize_t haveRead = 0;
    while (true) {
        bool b = waitReadable(this->timeout);
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

ssize_t SerialLinux::write(uchar *buf, ssize_t size) const {
    ssize_t i = ::write(this->fd, buf, size);
    if (i == -1) throw String("write error");
    return i;
}

unsigned int SerialLinux::getBaud() const {
    return this->baud;
}

void SerialLinux::flush() const {
    if (tcflush(this->fd, TCIOFLUSH)) throw String("flush error");
}

void SerialLinux::setTimeout(uint32_t t) {
    this->timeout = t;
}

void SerialLinux::setParity(char p) {
    this->parity = p;
}

char SerialLinux::getParity() const {
    return this->parity;
}

uint32_t SerialLinux::getTimeout() const {
    return this->timeout;
}

String SerialLinux::getPortName() const {
    return this->portName;
}
