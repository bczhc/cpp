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
#include <cerrno>
//#include "valgrind/valgrind.h"
#include "./third_party/practice/SymbolTable.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;
using namespace bczhc::array;
using namespace concurrent;
using namespace symboltable;

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

    int mLevel;

    void basicConfig(int level) {
        mLevel = level;
    }

    void debug(const char *format, ...) const {
        if (mLevel == DEBUG) {
            va_list args{};
            va_start(args, format);
            vfprintf(stdout, format, args);
            va_end(args);
        }
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
using ByteArray = Array<uchar>;

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

double time() {
    return ((double) (getCurrentTimeMillis())) / 1000;
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

timespec timespec_from_ms(const uint32_t millis) {
    timespec time{};
    time.tv_sec = millis / 1e3;
    time.tv_nsec = (millis - (time.tv_sec * 1e3)) * 1e6;
    return time;
}

bool waitReadable(int fd, uint32_t timeout) {
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

template<typename T>
class Tuple2 {
public:
    T a1{}, a2{};

    Tuple2() = default;

    Tuple2(T a1, T a2) : a1(a1), a2(a2) {}

    bool operator==(const Tuple2 &rhs) const {
        return a1 == rhs.a1 && a2 == rhs.a2;
    }

    bool operator!=(const Tuple2 &rhs) const {
        return !(rhs == *this);
    }

    T operator[](int index) const {
        if (index == 0) return a1;
        if (index == 1) return a2;
        return T{};
    }
};

// byte tuple
using Tuple2B = Tuple2<uchar>;
//string tuple
using Tuple2S = Tuple2<const char *>;
// SymbolTable byte-string
using SymbolTableBS = SymbolTable<Tuple2B, Tuple2S>;

class Serial {
private:
    int fd;
    uint32_t baud = 0;
    uint32_t timeout;
    char parity = Serial::PARITY_NONE;


    explicit Serial(int fd, uint32_t baud, uint32_t timeout) {
        this->fd = fd;
        setSpeed(9600);
        setSpeed(baud);
        this->timeout = timeout;
    }

public:
    static inline char PARITY_NONE = 'N';
    static inline char PARITY_EVEN = 'E';
    static inline char PARITY_ODD = 'O';
    static inline char PARITY_MARK = 'M';
    static inline char PARITY_SPACE = 'S';

    static Serial open(const char *port, uint32_t timeout = -1) {
        int o = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (o == -1) throw String("Open port failed.");
        auto s = Serial(o, 9600, timeout);
        return s;
    }

    void setSpeed(unsigned int speed) {
        unsigned int speedArr[] = {B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600,
                                   B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600,
                                   B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000};
        unsigned int nameArr[] = {0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400,
                                  57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000,
                                  2000000, 2500000, 3000000, 3500000, 4000000};
        termios opt{};
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
                baud = speed;
                return;
            }
            tcflush(fd, TCIOFLUSH);
        }
        throw String("Setting speed failed.");
    }

    void close() const {
        if (::close(fd) != 0) throw String("Close failed.");
    }

    void read(uchar *buf, ssize_t size) const {
        ssize_t count = 0;
        while (count < size) {
            bool b = waitReadable(fd, timeout);
            if (b) {
                count += ::read(fd, buf, size);
            } else {
                // timeout occurred
                break;
            }
        }
    }

    ssize_t write(uchar *buf, ssize_t size) const {
        ssize_t i = ::write(fd, buf, size);
        if (i == -1) throw String("write error");
        return i;
    }

    [[nodiscard]] unsigned int getBaud() const {
        return baud;
    }

    [[nodiscard]] int getFileDescriptor() const {
        return fd;
    }

    void flush() const {
        // `::write()` has no cache, so there's no need to "flush".
    }

    void setTimeout(uint32_t t) {
        this->timeout = t;
    }

    void setParity(char p) {
        this->parity = p;
    }
};

void autoisp(Serial &conn, int baud, NonableString &magic) {
    if (magic.isNone) return;
    uint32_t bak = conn.getBaud();
    conn.setSpeed(baud);

    String &magicStr = magic.var;
    size_t size = magicStr.size();
    const char *data = magicStr.getCString();
    conn.write((uchar *) data, size);
    conn.flush();
    Thread::sleep(500);
    conn.setSpeed(bak);
}

template<typename T1, typename T2>
bool in(T1 a, T2 *set, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        if (a == set[i]) return true;
    }
    return false;
}

class Programmer {
public:
    Serial &conn;
    NonableString &protocol;
    int chkmode = 0;

    Programmer(Serial &conn, NonableString &protocol) : conn(conn), protocol(protocol) {
        conn.setTimeout(50);
        if (in<String, const char *>(this->protocol.var, PROTOSET_PARITY,
                                     sizeof(PROTOSET_PARITY) / sizeof(PROTOSET_PARITY[0]))) {
            conn.setParity(Serial::PARITY_EVEN);
        } else conn.setParity(Serial::PARITY_NONE);
        this->chkmode = 0;
    }

    Array<uchar> __conn_read(ssize_t size) {
        SequentialList<uchar> buf;
        while (buf.length() < size) {
            ssize_t readLen = size - buf.length();
            uchar s[readLen];
            this->conn.read(s, readLen);
            buf.insert(buf.length(), s, readLen);
            //TODO debug msg
        }
        int len = buf.length();
        Array<uchar> r(len);
        for (int i = 0; i < len; ++i) {
            r[i] = buf[i];
        }
        return r;
    }

    void __conn_write(uchar *buf, ssize_t size) {
        //TODO debug msg
        this->conn.write(buf, size);
    }

    void __conn_baudrate(uint32_t baud, bool flush = true) {
        //TODO debug msg
        if (flush) {
            conn.flush();
            Thread::sleep(200);
        }
        conn.setSpeed(baud);
    }

    template<typename T1, typename T2>
    class Bean2 {
    public:
        T1 a1;
        T2 a2;

        Bean2() = default;

        Bean2(T1 a1, T2 a2) : a1(a1), a2(a2) {}
    };

    Bean2<String, int> __model_database(const Array<uchar> &model) {
        class Bean {
        public:
            const char *s = nullptr;
            int i = 0;
            SymbolTableBS *st = nullptr;

            Bean(const char *s, int i, SymbolTableBS &st) : s(s), i(i), st(&st) {}

            Bean() = default;
        };

        // SymbolTable byte-Bean
        using SymbolTableBB = SymbolTable<uchar, Bean>;

        SymbolTableBB modelMap;
        SymbolTableBS st[10];
        Bean bean[10];

        const char *d1[] = {"12", "12", "11", "12", "12", "10", "11", "89", "89", "12"};
        int d2[] = {1, 1, 1, 1, 2, 1, 2, 4, 4, 1};
        uchar mapKeys[] = {0xE0, 0xE1, 0xE2, 0xE6, 0xD1, 0xD2, 0xD3, 0xF0, 0xF1, 0xF2};

        st[0].put(Tuple2B(0x00, 0x1F), Tuple2S("C54", ""));
        st[0].put(Tuple2B(0x60, 0x7F), Tuple2S("C54", "AD"));
        st[0].put(Tuple2B(0x80, 0x9F), Tuple2S("LE54", ""));
        st[0].put(Tuple2B(0xE0, 0xFF), Tuple2S("LE54", "AD"));

        st[1].put(Tuple2B(0x00, 0x1F), Tuple2S("C52", ""));
        st[1].put(Tuple2B(0x20, 0x3F), Tuple2S("C52", "PWM"));
        st[1].put(Tuple2B(0x60, 0x7F), Tuple2S("C52", "AD"));
        st[1].put(Tuple2B(0x80, 0x9F), Tuple2S("LE52", ""));
        st[1].put(Tuple2B(0xA0, 0xBF), Tuple2S("LE52", "PWM"));
        st[1].put(Tuple2B(0xE0, 0xFF), Tuple2S("LE52", "AD"));

        st[2].put(Tuple2B(0x00, 0x1F), Tuple2S("F", ""));
        st[2].put(Tuple2B(0x20, 0x3F), Tuple2S("F", "E"));
        st[2].put(Tuple2B(0x70, 0x7F), Tuple2S("F", ""));
        st[2].put(Tuple2B(0x80, 0x9F), Tuple2S("L", ""));
        st[2].put(Tuple2B(0xA0, 0xBF), Tuple2S("L", "E"));
        st[2].put(Tuple2B(0xF0, 0xFF), Tuple2S("L", ""));

        st[3].put(Tuple2B(0x00, 0x1F), Tuple2S("C56", ""));
        st[3].put(Tuple2B(0x60, 0x7F), Tuple2S("C56", "AD"));
        st[3].put(Tuple2B(0x80, 0x9F), Tuple2S("LE56", ""));
        st[3].put(Tuple2B(0xE0, 0xFF), Tuple2S("LE56", "AD"));

        st[4].put(Tuple2B(0x20, 0x3F), Tuple2S("C5A", "CCP"));
        st[4].put(Tuple2B(0x40, 0x5F), Tuple2S("C5A", "AD"));
        st[4].put(Tuple2B(0x60, 0x7F), Tuple2S("C5A", "S2"));
        st[4].put(Tuple2B(0xA0, 0xBF), Tuple2S("LE5A", "CCP"));
        st[4].put(Tuple2B(0xC0, 0xDF), Tuple2S("LE5A", "AD"));
        st[4].put(Tuple2B(0xE0, 0xFF), Tuple2S("LE5A", "S2"));

        st[5].put(Tuple2B(0x00, 0x0F), Tuple2S("F", ""));
        st[5].put(Tuple2B(0x60, 0x6F), Tuple2S("F", "XE"));
        st[5].put(Tuple2B(0x70, 0x7F), Tuple2S("F", "X"));
        st[5].put(Tuple2B(0xA0, 0xAF), Tuple2S("L", ""));
        st[5].put(Tuple2B(0xE0, 0xEF), Tuple2S("L", "XE"));
        st[5].put(Tuple2B(0xF0, 0xFF), Tuple2S("L", "X"));

        st[6].put(Tuple2B(0x00, 0x1F), Tuple2S("F", ""));
        st[6].put(Tuple2B(0x40, 0x5F), Tuple2S("F", "X"));
        st[6].put(Tuple2B(0x60, 0x7F), Tuple2S("F", "XE"));
        st[6].put(Tuple2B(0xA0, 0xBF), Tuple2S("L", ""));
        st[6].put(Tuple2B(0xC0, 0xDF), Tuple2S("L", "X"));
        st[6].put(Tuple2B(0xE0, 0xFF), Tuple2S("L", "XE"));

        st[7].put(Tuple2B(0x00, 0x10), Tuple2S("C5", "RC"));
        st[7].put(Tuple2B(0x20, 0x30), Tuple2S("C5", "RC"));// STC90C5xR

        st[8].put(Tuple2B(0x00, 0x10), Tuple2S("C5", "RD+"));
        st[8].put(Tuple2B(0x20, 0x30), Tuple2S("C5", "RD+"));// STC90C5xRD

        st[9].put(Tuple2B(0x00, 0x0F), Tuple2S("C", "052"));
        st[9].put(Tuple2B(0x10, 0x1F), Tuple2S("C", "052AD"));
        st[9].put(Tuple2B(0x20, 0x2F), Tuple2S("LE", "052"));
        st[9].put(Tuple2B(0x30, 0x3F), Tuple2S("LE", "052AD"));

        for (int i = 0; i < 10; ++i) {
            bean[i].st = &(st[i]);
            bean[i].s = d1[i];
            bean[i].i = d2[i];

            modelMap.put(mapKeys[i], bean[i]);
        }

        uchar iapmcu[9][2] = {
                {0xD1, 0x3F},
                {0xD1, 0x5F},
                {0xD1, 0x7F},
                {0xD2, 0x7E},
                {0xD2, 0xFE},
                {0xD3, 0x5F},
                {0xD3, 0xDF},
                {0xE2, 0x76},
                {0xE2, 0xF6}};

        SymbolTableBB::Result result = modelMap.get(model.elements[0]);
        if (!result.found) throw String("not found");
        const char *prefix = result.val.s;
        int romratio = result.val.i;
        SymbolTableBS *fixmap = result.val.st;
        if ((model[0] == 0xF0 || model[0] == 0xF1) && (0x20 <= model[1] && model[1] <= 0x30)) {
            prefix = "90";
        }
        SymbolTable<Tuple2B, Tuple2S>::Iterator it = fixmap->getIterator();
        Tuple2S foundVal;
        Tuple2B foundKey;
        bool broken = false;
        while (it.hasNext()) {
            const SymbolTable<Tuple2B, Tuple2S>::Bean b = it.next();
            if (b.key[0] <= model[1] && model[1] <= b.key[1]) {
                broken = true;
                foundVal = b.val, foundKey = b.key;
                break;
            };
        }
        if (!broken) {
            throw String("key error");
        }
        const char *infix = foundVal[0], *postfix = foundVal[1];
        int romsize = romratio * (model[1] - foundKey[0]);

        try {
            if (model.length() != 2) throw 1;
            if (model.elements[0] != 0xF0 || model.elements[1] != 0x03) throw 1;
            romsize = 13;
        } catch (...) {
        }
        String romfix;
        if (model[0] == 0xF0 && model[1] == 0xF1) {
            romfix = String::toString(model[1] - foundKey[0]);
        } else if (model[0] == 0xF2) romfix = String::toString(romsize);
        else {
            String s = String::toString(romsize);
            if (s.utf8Length() == 0) s = String("0") + s;
        }
        String name = "IAP";
        for (auto &j : iapmcu) {
            if (j[0] != model[0] || j[1] != model[1]) {
                name = "STC";
                break;
            }
        }
        name = name + prefix + infix + romfix + postfix;
        return Bean2<String, int>(name, romsize);
    }

    Bean2<uchar, Array<uchar>> recv(double timeout = 1.0, Array<uchar> start = Array<uchar>(0)) {
        if (start.length() == 0) {
            start = Array<uchar>(3);
            start[0] = 0x46, start[1] = 0xB9, start[2] = 0x68;
        }
        timeout += time();
        bool broken = false;
        while (time() < timeout) {
            try {
                const Array<uchar> connRead = __conn_read(start.length());
                if (connRead == start) {
                    broken = true;
                    break;
                }
            } catch (...) {
                continue;
            }
        }
        if (!broken) {
            logging.debug("%s\n", "recv(..): Timeout");
            throw String("io error");
        }

        uchar chksum = start[start.length() - 1];
        Array<uchar> s = __conn_read(2);
        int n = s[0] * 256 + s[1];
        if (n > 64) {
            logging.debug("recv(..): Incorrect packet size");
            throw String("io error");
        }
        chksum += sum<uchar, uchar>(s);

        s = __conn_read(n - 3);
        if (s[n - 4] != 0x16) {
            logging.debug("recv(..): Missing terminal symbol");
            throw String("io error");
        }
        int newLen = s.length() - (1 + this->chkmode);
        Array<uchar> t(newLen);
        for (int i = 0; i < newLen; ++i) {
            t[i] = s[i];
        }
        chksum += sum<uchar, uchar>(t);
        if (this->chkmode > 0 && ((chksum & 0xFF) != s[s.length() - 2])) {
            logging.debug("recv(..): Incorrect checksum[0]");
            throw String("io error");
        } else if (this->chkmode > 1 && ((chksum >> 8) & 0xFF) != s[s.length() - 3]) {
            logging.debug("recv(..): Incorrect checksum[1]");
            throw String("io error");
        }
        newLen = s.length() - (1 + this->chkmode) - 1;
        Array<uchar> r(newLen);
        for (int i = 0; i < newLen; ++i) {
            r[i] = s[i + 1];
        }
        return Bean2<uchar, Array<uchar>>(s[0], r);
    }

    void detect() {
        uchar buf[] = {0x7F, 0x7F};
        bool broken = false;
        const Array<uchar> start(1);
        start[0] = 0x68;

        for (int i = 0; i < 1000; ++i) {
            try {
                __conn_write(buf, 2);
                const Bean2 <uchar, Array<uchar>> recvBean = recv(0.015, start);
                uchar cmd = recvBean.a1;
                Array<uchar> dat = recvBean.a2;
                broken = true;
                break;
            } catch (...) {
            }
        }
        if (!broken) {
            throw String("io error");
        }
    }
};

void program(Programmer &prog, Code &code, NonableBoolean erase_eeprom = NonableBoolean(true, true)) {
    printf("%s", "Detecting target...");
    fflush(stdout);

    prog.detect();
}

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
    opts.image = new InputStream(filename);

    opts.loglevel = Logging::combination[min(2, opts.verbose)];
    opts.protocol = chooseProtocol(opts.protocol.var.getCString());

    if (!opts.erase_eeprom.var && !opts.not_erase_eeprom) {
        opts.erase_eeprom.isNone = true;
    }

    logging.basicConfig(opts.loglevel);

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
    printf("Connect to %s at baudrate %d\n", opts.port.getCString(), opts.lowbaud);
    Serial conn = Serial::open(opts.port.getCString());
    if (!opts.aispmagic.isNone) autoisp(conn, opts.aispbaud, opts.aispmagic);
    Programmer programmer(conn, opts.protocol);
    program(programmer, code, opts.erase_eeprom);
    return 0;
}

void m() {
}

int main(int argc, char **argv) {
    goto a;
    try {
        m();
    } catch (const String &e) {
        printf("%s\n", e.getCString());
    }
    return 0;
    a:;
    int status = 1;
    try {
        status = run();
    } catch (const String &e) {
        fprintf(stderr, "%s\n", e.getCString());
        return status;
    }
    return status;
}

#pragma clang diagnostic pop
