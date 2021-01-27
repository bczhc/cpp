#include <termios.h>
#include "./string.hpp"
#include "file.h"
#include "io.h"
#include "utils.hpp"
#include "array.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "concurrent.h"
#include <cstdarg>
//#include "valgrind/valgrind.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;
using namespace bczhc::array;
using namespace concurrent;

static const char *PROTOCOL_89 = "89";
static const char *PROTOCOL_12C5A = "12c5a";
static const char *PROTOCOL_12C52 = "12c52";
static const char *PROTOCOL_12Cx052 = "12cx052";
static const char *PROTOSET_89[] = {PROTOCOL_89};
static const char *PROTOSET_12[] = {PROTOCOL_12C5A, PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_12B[] = {PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_PARITY[] = {PROTOCOL_12C5A, PROTOCOL_12C52};

#define COMPARE_CODE0 0;

String getPort() {
    String platform;
#ifdef _WIN32
    platform = "win32";
#endif
#ifdef __APPLE__
    platform = "darwin";
#endif

    if (platform.equals("win32")) {
        return "COM3";
    } else if (platform.equals("darwin")) {
        return "/dev/tty.usbserial";
    }
    return "/dev/ttyUSB0";
}

class Logging {
public:
    inline static int CRITICAL = 50;
    inline static int INFO = 20;
    inline static int DEBUG = 10;

    inline static int combination[] = {CRITICAL, INFO, DEBUG};

    void debug(const char *format, ...) {

    }
};

static Logging logging;

class None {
    int code = COMPARE_CODE0;
};

template<typename T>
class TypeWithNone {
public:
    T var;
    bool isNone = false;

    explicit TypeWithNone(T a) : var(a) {}

    explicit TypeWithNone(T a, bool isNone) : var(a), isNone(isNone) {}
};

using uchar = unsigned char;
using NonableString = TypeWithNone<String>;
using NonableBoolean = TypeWithNone<bool>;
template<typename T>
using NonableArray = TypeWithNone<Array<T>>;
using Code = NonableArray<uchar>;

TypeWithNone<String> chooseProtocol(const char *protocolOrigin) {
    if (String::equal("89", protocolOrigin)) return NonableString(PROTOCOL_89);
    if (String::equal("12c5a", protocolOrigin)) return NonableString(PROTOCOL_12C5A);
    if (String::equal("12c52", protocolOrigin)) return NonableString(PROTOCOL_12C52);
    if (String::equal("12cx052", protocolOrigin)) return NonableString(PROTOCOL_12Cx052);
    if (String::equal("auto", protocolOrigin)) return NonableString(nullptr, true);
    return NonableString(nullptr, true);
}

/**
 * TODO optimize
 * @param in 
 * @return 
 */
Code readToBytes(InputStream &in) {
    SequentialList<unsigned char> b;
    char buf[4096];
    int readLen;
    while ((readLen = in.read(buf, 4096)) > 0) {
        for (int i = 0; i < readLen; ++i) {
            b.insert(buf[i]);
        }
    }
    int size = b.length();
    Array<unsigned char> r(size);
    for (int i = 0; i < size; ++i) {
        r[i] = b.data[i];
    }
    return Code(r);
}

Array<unsigned char> hex2bytes(const String &hexString) {
    int i = (int) hexString.size();
    int len = i / 2;
    Array<unsigned char> dat(len);
    for (int j = 0; j < len; ++j) {
        const String &s = hexString.substr(j * 2, 2);
        dat[j] = (unsigned char) parseInt(s, 16);
    }
    return dat;
}

template<typename ReturnType, typename ElementType>
ReturnType sum(Array<ElementType> arr) {
    ReturnType r = 0;
    int len = arr.length();
    for (int i = 0; i < len; ++i) {
        r += arr[i];
    }
    return r;
}

template<typename T>
void sliceAssign(SequentialList<T> &list, int start, int end, const T *valueToAssign, int length) {
    list.remove(start, end);
    list.insert(start, valueToAssign, length);
}

Code hex2bin(Code &code) {
    SequentialList<unsigned char> buf;
    int base = 0, line = 0;

    SequentialList<String> lines;
    String lineStr = String();
    int codeLen = code.var.length();
    for (int j = 0; j < codeLen; ++j) {
        char c = code.var[j];
        if (c != '\n') lineStr.append(c);
        else {
            lines.insert(lineStr);
            lineStr = String();
        }
    }

    int len = lines.length();
    for (int i = 0; i < len; ++i) {
        auto rec = lines.get(i);
        line += 1;
        char n = hex2bytes(rec.substring(1, 3))[0];
        Array<unsigned char> dat = hex2bytes(rec.substring(1, n * 2 + 11));
        if (rec.charAt(0) != ':')
            throw String("Line ") + String::toString(line) + ": Missing start code \":\"";
        if ((sum<int, unsigned char>(dat) & 0xFF) != 0)
            throw String("Line ") + String::toString(line) + ": Incorrect checksum";
        if (dat[3] == 0) {
            // Data record
            int addr = base + (dat[1] << 8) + dat[2];
            // Allocate memory space and fill it with 0xFF
            int size = addr + n - buf.length();
            size = size < 0 ? 0 : size;
            unsigned char t[size];
            memset(t, 0xFF, size);
            sliceAssign(buf, buf.length(), buf.length(), t, size);
            // Copy data to the buffer
            sliceAssign(buf, addr, addr + n, dat.elements + 4, dat.length() - 1 - 4);
        } else if (dat[3] == 1) {
            // EOF record
            if (n != 0) throw String("Line ") + String::toString(line) + ": Incorrect data length";
        } else if (dat[3] == 2) {
            // Extended segment address record
            if (n != 2) throw String("Line ") + String::toString(line) + ": Incorrect data length";
            base = ((dat[4] << 8) + dat[5]) << 4;
        } else if (dat[3] == 4) {
            if (n != 2) throw String("Line ") + String::toString(line) + ": Incorrect data length";
            base = ((dat[4] << 8) + dat[5]) << 16;
        } else throw String("Line ") + String::toString(line) + ": Unsupported record type";
    }
    Array<unsigned char> r(buf.length());
    for (int i = 0; i < r.length(); ++i) {
        r[i] = buf[i];
    }
    return Code(r);
}


class Serial {
private:
    int fd;

    unsigned int baud = 0;

    Serial(int fd) : fd(fd) {}

public:
    static Serial open(const char *port) {
        int o = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (o == -1) throw String("Open failed.");
        return Serial(o);
    }

    void setSpeed(unsigned int speed) {
        unsigned int speedArr[] = {B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600,
                                   B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600,
                                   B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000};
        unsigned int nameArr[] = {0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400,
                                  57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000,
                                  2000000, 2500000, 3000000, 3500000, 4000000};
        termios opt;
        tcgetattr(fd, &opt);
        int len = (int) (sizeof(speedArr) / sizeof(speedArr[0]));
        for (int i = 0; i < len; i++) {
            if (speed == nameArr[i]) {
                tcflush(fd, TCIOFLUSH);
                unsigned int rate = speedArr[i];
                cfsetispeed(&opt, rate);
                cfsetospeed(&opt, rate);
                int status = tcsetattr(fd, TCSANOW, &opt);
                if (status != 0) throw String("tcsetattr fd1");
                baud = rate;
                return;
            }
            tcflush(fd, TCIOFLUSH);
        }
        throw String("Setting speed failed.");
    }

    int close() const {
        return ::close(fd);
    }

    void read(char *buf, ssize_t size) const {
        char ch;
        ssize_t count = 0;
        for (;;) {
            ssize_t i = ::read(fd, &ch, size);
            if (i > 0) {
                buf[count] = ch;
                count += i;
                if (count == size) break;
            } else if (i == -1) throw String("read error");
        }
    }

    ssize_t write(const char *buf, ssize_t size) const {
        ssize_t i = ::write(fd, buf, size);
        if (i == -1) throw String("write error");
        return i;
    }

    unsigned int getBaudRate() const {
        return baud;
    }

    void flush() const {
        // `::write()` has no cache, so there's no need to "flush".
    }
};

void autoisp(Serial &conn, int baud, NonableString magic) {
    if (magic.isNone) return;
    uint32_t bak = conn.getBaudRate();
    conn.setSpeed(baud);

    String &magicStr = magic.var;
    size_t size = magicStr.size();
    const char *data = magicStr.getCString();
    conn.write(data, size);
    conn.flush();
    Thread::sleep(500);
    conn.setSpeed(bak);
}

class Programmer {
public:
    Serial &conn;
    NonableString &protocol;

    Programmer(Serial &conn, NonableString &aProtocol) : conn(conn), protocol(aProtocol) {}

};

int run() {
    struct Opt {
        uint32_t aispbaud = 4800;
        NonableString aispmagic = NonableString(nullptr, true);
        NonableBoolean erase_eeprom = NonableBoolean(false);
        InputStream *image = nullptr;
        uint32_t lowbaud = 2400;
        bool not_erase_eeprom = false;
        String port = "/dev/ttyUSB0";
        NonableString protocol = NonableString("auto");
        int verbose = 0;
        int loglevel = 0;
    } opts;

    opts.port = getPort();

    String filename = "/home/zhc/stc/a.hex";
    try {
        opts.image = new InputStream(filename);
    } catch (const String &e) {
        printf("%s\n", e.getCString());
        return 1;
    }

    opts.loglevel = Logging::combination[min(2, opts.verbose)];
    opts.protocol = chooseProtocol(opts.protocol.var.getCString());

    if (!opts.erase_eeprom.var && !opts.not_erase_eeprom) {
        opts.erase_eeprom.isNone = true;
        //TODO missing routines

        Code code(Array<uchar>(0));
        if (opts.image != nullptr) {
            Code localCode = readToBytes(*opts.image);
            opts.image->close();
            delete opts.image;
            String extension = File::getExtension(filename);
            if (extension.equals("hex") || extension.equals("ihx")) {
                localCode = hex2bin(localCode);
            }
            code.var = localCode.var;
        } else code.isNone = true;
    }
    printf("Connect to %s at baudrate %d\n", opts.port.getCString(), opts.lowbaud);
    Serial conn = Serial::open(opts.port.getCString());
    if (!opts.aispmagic.isNone) autoisp(conn, opts.aispbaud, opts.aispmagic);
    return 0;
}

void m() {

}

int main(int argc, char **argv) {
    int status = 1;
    try {
        status = run();
    } catch (const String &e) {
        printf("%s\n", e.getCString());
    }
    return status;
}

#pragma clang diagnostic pop
