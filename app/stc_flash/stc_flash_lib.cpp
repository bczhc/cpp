#include <iostream>

using namespace std;
#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "serial.h"
#include "stc_flash_lib.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

static const char *PROTOCOL_89 = "89";
static const char *PROTOCOL_12C5A = "12c5a";
static const char *PROTOCOL_12C52 = "12c52";
static const char *PROTOCOL_12Cx052 = "12cx052";
static const char *PROTOSET_89[] = {PROTOCOL_89};
static const char *PROTOSET_12[] = {PROTOCOL_12C5A, PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_12B[] = {PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_PARITY[] = {PROTOCOL_12C5A, PROTOCOL_12C52};

static char echoMsg[100];
static EchoCallback *echo;

class Logging {
public:
    constexpr static int CRITICAL = 50;
    constexpr static int INFO = 20;
    constexpr static int DEBUG = 10;

    constexpr static int combination[] = {CRITICAL, INFO, DEBUG};

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

    void info(const char *format, ...) const {
        if (mLevel == INFO) {
            va_list args{};
            va_start(args, format);
            vfprintf(stdout, format, args);
            va_end(args);
        }
    }
};

void echoPrint(const char *format, ...) {
    va_list args{};
    va_start(args, format);
    char s[201];
    vsnprintf(s, 200, format, args);
    s[200] = '\0';
    echo->print(s);
    va_end(args);
}

static Logging logging;

template<typename T>
class TypeWithNone {
public:
    T val;
    bool isNone = false;

    explicit TypeWithNone(T a) : val(a) {}

    explicit TypeWithNone(T a, bool isNone) : val(a), isNone(isNone) {}
};

using NonableString = TypeWithNone<String>;
using NonableBoolean = TypeWithNone<bool>;
template<typename T>
using NonableArray = TypeWithNone<Array<T>>;
using Code = NonableArray<uchar>;
using NonableInt = TypeWithNone<int>;

TypeWithNone<String> chooseProtocol(const char *protocolOrigin) {
    if (String::equal("89", protocolOrigin)) return NonableString(PROTOCOL_89);
    if (String::equal("12c5a", protocolOrigin)) return NonableString(PROTOCOL_12C5A);
    if (String::equal("12c52", protocolOrigin)) return NonableString(PROTOCOL_12C52);
    if (String::equal("12cx052", protocolOrigin)) return NonableString(PROTOCOL_12Cx052);
    if (String::equal("auto", protocolOrigin)) return NonableString(nullptr, true);
    return NonableString(nullptr, true);
}

template<typename T>
Array<T> list2arr(SequentialList<T> &sl) {
    int length = sl.length();
    Array<T> r(length);
    for (int i = 0; i < length; ++i) {
        r[i] = sl[i];
    }
    return r;
}

template<typename T>
void repeatListInsert(SequentialList<T> &list, T value, int times) {
    for (int i = 0; i < times; ++i) {
        list.insert(value);
    }
}

template<typename T>
void insertArrToList(SequentialList<T> &list, T *arr, int size) {
    for (int i = 0; i < size; ++i) {
        list.insert((T) arr[i]);
    }
}

template<typename T>
void insertArrToList(SequentialList<T> &list, const Array<T> &arr) {
    for (int i = 0; i < arr.length(); ++i) {
        list.insert((T) arr[i]);
    }
}

Code readToBytes(InputStream &in) {
    SequentialList<unsigned char> b;
    char buf[4096];
    int readLen;
    while ((readLen = in.read(buf, 4096)) > 0) {
        for (int i = 0; i < readLen; ++i) {
            b.insert(buf[i]);
        }
    }
    return Code(list2arr<uchar>(b));
}

double time() {
    return ((double) (getCurrentTimeMillis())) / 1000;
}

Array<unsigned char> hex2bytes(const String &hexString) {
    int i = (int) hexString.length();
    int len = i / 2;
    Array<unsigned char> dat(len);
    for (int j = 0; j < len; ++j) {
        const String &s = hexString.substr(j * 2, 2);
        dat[j] = (unsigned char) Integer::parseInt(s, 16);
    }
    return dat;
}

template<typename ReturnType, typename ElementType>
ReturnType sum(ElementType *arr, int start, int end) {
    ReturnType r = 0;
    for (int i = start; i < end; ++i) {
        r += arr[i];
    }
    return r;
}

template<typename ReturnType, typename ElementType>
ReturnType sum(Array<ElementType> arr, int start, int end) {
    return sum<ReturnType, ElementType>(arr.elements, start, end);
}

template<typename ReturnType, typename ElementType>
ReturnType sum(Array<ElementType> arr) {
    return sum<ReturnType, ElementType>(arr, 0, arr.length());
}

void checkAndSetBoundary(int &start, int &end, int length) {
    if (start < 0) start += length;
    if (end < 0) end += length;

    if (start < 0) start = 0;
    else if (start > length)
        start = length;
    if (end < 0) end = 0;
    else if (end > length)
        end = length;
}

template<typename T>
void sliceAssign(SequentialList<T> &list, int start, int end, const T *valueToAssign, int length) {
    checkAndSetBoundary(start, end, list.length());
    list.remove(start, end);
    list.insert(start, valueToAssign, length);
}

Array<uchar> packUnsignedInt32BigEndian(uint32_t a) {
    bool reverse = false;
    int i = 0x11223344;
    reverse = (*((uchar *) &i) != 0x11);
    int len = (int) sizeof(uint32_t);
    Array<uchar> r(len);
    auto *p = (uchar *) &a;
    for (int j = 0; j < len; ++j) {
        r[j] = p[reverse ? (len - 1 - j) : j];
    }
    return r;
}

Code hex2bin(Code &code) {
    SequentialList<unsigned char> buf;
    int base = 0, line = 0;

    SequentialList<String> lines;
    String lineStr = String();
    int codeLen = code.val.length();
    for (int j = 0; j < codeLen; ++j) {
        char c = code.val[j];
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
        uchar n = hex2bytes(rec.substring(1, 3))[0];
        Array<unsigned char> dat = hex2bytes(rec.substring(1, n * 2 + 11));
        if (rec.charAt(0) != ':')
            throw String("Line ") + String::toString(line) + ": Missing start code \":\"";
        if ((sum<int32_t, unsigned char>(dat) & 0xFF) != 0)
            throw String("Line ") + String::toString(line) + ": Incorrect checksum";
        if (dat[3] == 0) {
            // Data record
            int addr = base + (dat[1] << 8) + dat[2];
            // Allocate memory space and fill it with 0xFF
            int size = addr + n - buf.length();
            if (size < 0) size = 0;
            unsigned char t[size];
            memset(t, 0xFF, size);
            sliceAssign(buf, buf.length(), buf.length(), t, size);
            // Copy data to the buffer
            sliceAssign(buf, addr, addr + n, dat.elements + 4 * sizeof(uchar), dat.length() - 1 - 4);
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
        } else
            throw String("Line ") + String::toString(line) + ": Unsupported record type";
    }
    Array<unsigned char> r(buf.length());
    for (int i = 0; i < r.length(); ++i) {
        r[i] = buf[i];
    }
    return Code(r);
}

template<typename T>
String hexStrJoin(char joinMark, const Array<T> &arr) {
    String r;
    int length = arr.length();
    for (int i = 0; i < length; ++i) {
        String hex = String::toString(arr[i], 16);
        if (hex.utf8Length() == 1) hex = String("0") + hex;
        r += hex;
        if (i != length - 1) r += joinMark;
    }
    return r;
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
// string tuple
using Tuple2S = Tuple2<const char *>;
// Tuple SymbolTable byte-string
using SymbolTableTupleBS = SymbolTable<Tuple2B, Tuple2S>;
// SymbolTable byte-string
using SymbolTableBS = SymbolTable<uchar, const char *>;

void autoisp(serial::Serial &conn, int baud, NonableString &magic) {
    if (magic.isNone) return;
    uint32_t bak = conn.getBaud();
    conn.setSpeed(baud);

    String &magicStr = magic.val;
    size_t size = magicStr.length();
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

template<typename T>
Array<T> cutArray(const Array<T> &arr, int start, int end, int step = 1) {
    SequentialList<T> list;
    int len = arr.length();
    checkAndSetBoundary(start, end, len);
    for (int i = start; i < end; i += step) {
        list.insert(arr[i]);
    }
    return list2arr<T>(list);
}

template<typename T>
SequentialList<T> cutList(SequentialList<T> &list, int start, int end, int step = 1) {
    int len = list.length();
    checkAndSetBoundary(start, end, len);
    SequentialList<T> r;
    for (int i = start; i < end; i += step) {
        r.insert(list.get(i));
    }
    return r;
}

class Programmer {
public:
    serial::Serial &conn;
    NonableString &protocol;
    int chkmode = 0;
    double fosc = 0;
    Array<uchar> info = Array<uchar>(0);
    String version;
    Array<uchar> model = Array<uchar>(0);
    String name;
    NonableInt romsize = NonableInt(int{}, true);
    uint32_t bundrate = 0;


    Programmer(serial::Serial &conn, NonableString &protocol) : conn(conn), protocol(protocol) {
        conn.setTimeout(50);
        if (in<String, const char *>(this->protocol.val, PROTOSET_PARITY, ARR_SIZE(PROTOSET_PARITY))) {
            conn.setParity(serial::Serial::PARITY_EVEN);
        } else
            conn.setParity(serial::Serial::PARITY_NONE);
        this->chkmode = 0;
    }

    [[nodiscard]] Array<uchar> __conn_read(ssize_t size) const {
        SequentialList<uchar> buf;
        while (buf.length() < size) {
            const Array<uchar> r = conn.read(size - buf.length());
            buf.insert(buf.length(), r.elements, r.length());
            //TODO debug msg
            if (buf.length() == 0) throw String("io error");
        }
        return list2arr<uchar>(buf);
    }

    void __conn_write(uchar *buf, ssize_t size) const {
        //TODO debug msg
        conn.write(buf, size);
    }

    void __conn_baudrate(uint32_t baud, bool flush = true) const {
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

    static Bean2<String, int> __model_database(const Array<uchar> &model) {
        class Bean {
        public:
            const char *s = nullptr;
            int i = 0;
            SymbolTableTupleBS *st = nullptr;

            Bean(const char *s, int i, SymbolTableTupleBS &st) : s(s), i(i), st(&st) {}

            Bean() = default;
        };

        // SymbolTable byte-Bean
        using SymbolTableBB = SymbolTable<uchar, Bean>;

        SymbolTableBB modelMap;
        SymbolTableTupleBS st[10];
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
        SymbolTableTupleBS *fixmap = result.val.st;
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
        int localRomsize = romratio * (model[1] - foundKey[0]);

        try {
            if (model.length() != 2) throw 1;
            if (model.elements[0] != 0xF0 || model.elements[1] != 0x03) throw 1;
            localRomsize = 13;
        } catch (...) {
        }
        String romfix;
        if (model[0] == 0xF0 || model[1] == 0xF1) {
            romfix = String::toString(model[1] - foundKey[0]);
        } else if (model[0] == 0xF2)
            romfix = String::toString(localRomsize);
        else {
            String s = String::toString(localRomsize);
            if (s.utf8Length() == 0) s = String("0") + s;
        }
        String localName = "IAP";
        for (auto &j : iapmcu) {
            if (j[0] != model[0] || j[1] != model[1]) {
                localName = "STC";
                break;
            }
        }
        localName = localName + prefix + infix + romfix + postfix;
        return Bean2<String, int>(localName, localRomsize);
    }

    [[nodiscard]] Bean2<uchar, Array<uchar>> recv(double timeout = 1.0, Array<uchar> start = Array<uchar>(0)) const {
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
            logging.debug("recv(..): Incorrect packet size\n");
            throw String("io error");
        }
        chksum += sum<int32_t, uchar>(s);


        s = __conn_read(n - 3);
        if (s[n - 4] != 0x16) {
            logging.debug("recv(..): Missing terminal symbol\n");
            throw String("io error");
        }
        chksum += sum<int32_t, uchar>(cutArray<uchar>(s, 0, -(1 + this->chkmode)));
        if (this->chkmode > 0 && ((chksum & 0xFF) != s[s.length() - 2])) {
            logging.debug("recv(..): Incorrect checksum[0]\n");
            throw String("io error");
        } else if (this->chkmode > 1 && ((chksum >> 8) & 0xFF) != s[s.length() - 3]) {
            logging.debug("recv(..): Incorrect checksum[1]\n");
            throw String("io error");
        }
        return Bean2<uchar, Array<uchar>>(s[0], cutArray<uchar>(s, 1, -(1 + this->chkmode)));
        ;
    }

    void detect() {
        uchar buf[] = {0x7F, 0x7F};
        bool broken = false;
        const Array<uchar> start(1);
        start[0] = 0x68;

        Array<uchar> dat(0);
        for (int i = 0; i < 1000; ++i) {
            try {
                __conn_write(buf, 2);
                const Bean2<uchar, Array<uchar>> recvBean = recv(0.015, start);
                uchar cmd = recvBean.a1;
                dat = recvBean.a2;
                broken = true;
                break;
            } catch (...) {
            }
        }
        if (!broken) {
            throw String("io error");
        }

        fosc = ((double) (sum<int32_t, uchar>(cutArray<uchar>(dat, 0, 16, 2)) * 256 +
                          sum<int32_t, uchar>(cutArray<uchar>(dat, 1, 16, 2)))) /
               8 * conn.getBaud() / 580974;
        this->info = cutArray<uchar>(dat, 16, dat.length());
        this->version = String::toString(info[0] >> 4) + '.' + String::toString(info[0] & 0x0F) + ((char) info[1]);
        this->model = cutArray<uchar>(info, 3, 5);
        const Bean2<String, int> bean = __model_database(model);
        this->name = bean.a1, this->romsize = NonableInt(bean.a2);
        logging.info("Model ID: %02X %02X\n", model[0], model[1]);
        logging.info("Model name: %s\n", name.getCString());
        logging.info("ROM size: %d\n", romsize.val);
        if (this->protocol.isNone) {
            try {
                SymbolTableBS m;
                m.put(0xF0, PROTOCOL_89);     // STC89/90C5xRC
                m.put(0xF1, PROTOCOL_89);     // STC89/90C5xRD+
                m.put(0xF2, PROTOCOL_12Cx052);// STC12Cx052
                m.put(0xD1, PROTOCOL_12C5A);  // STC12C5Ax
                m.put(0xD2, PROTOCOL_12C5A);  // STC10Fx
                m.put(0xE1, PROTOCOL_12C52);  // STC12C52x
                m.put(0xE2, PROTOCOL_12C5A);  // STC11Fx
                m.put(0xE6, PROTOCOL_12C52);  // STC12C56x
                const SymbolTableBS::Result result = m.get(model[0]);
                if (!result.found) throw String("key error");
                this->protocol = NonableString(result.val);
            } catch (...) {
            }

            if (!this->protocol.isNone &&
                in<String, const char *>(this->protocol.val, PROTOSET_PARITY, ARR_SIZE(PROTOSET_PARITY))) {
                this->chkmode = 2, conn.setParity(serial::Serial::PARITY_EVEN);
            } else {
                this->chkmode = 1, conn.setParity(serial::Serial::PARITY_NONE);
            }
            if (!this->protocol.isNone) {
                //del this->info
                logging.info("Protocol ID: %s\n", this->protocol.val.getCString());
                logging.info("Checksum mode: %d\n", this->chkmode);
                logging.info("UART Parity: %s\n", conn.getParity() == serial::Serial::PARITY_EVEN ? "EVEN" : "NONE");
                for (int i = 0; i < this->info.length(); i += 16) {
                    const Array<uchar> a = cutArray<uchar>(info, i, i + 16);
                    String s = hexStrJoin<uchar>(' ', a);
                    logging.info("Info string [%d]: %s\n", i / 16, s.getCString());
                }
            }
        }
    }

    void print_info() const {
        echoPrint(" FOSC: %.3fMHz\n", this->fosc);
        echoPrint(" Model: %s (ver%s) \n", this->name.getCString(), this->version.getCString());
        if (!this->romsize.isNone) {
            echoPrint(" ROM: %dKB\n", this->romsize.val);
        }

        class Bean {
        public:
            int pos{};
            uchar bit{};
            const char *desc{};

            Bean() = default;

            Bean(int pos, uchar bit, const char *desc) : pos(pos), bit(bit), desc(desc) {}
        };

        LinkedList<Bean> switches;
        if (this->protocol.val == PROTOCOL_89) {
            switches.insert(Bean(2, 0x80, "Reset stops watchdog"));
            switches.insert(Bean(2, 0x40, "Internal XRAM"));
            switches.insert(Bean(2, 0x20, "Normal ALE pin"));
            switches.insert(Bean(2, 0x10, "Full gain oscillator"));
            switches.insert(Bean(2, 0x08, "Not erase data EEPROM"));
            switches.insert(Bean(2, 0x04, "Download regardless of P1"));
            switches.insert(Bean(2, 0x01, "12T mode"));
        } else if (this->protocol.val == PROTOCOL_12C5A) {
            switches.insert(Bean(6, 0x40, "Disable reset2 low level detect"));
            switches.insert(Bean(6, 0x01, "Reset pin not use as I/O port"));
            switches.insert(Bean(7, 0x80, "Disable long power-on-reset latency"));
            switches.insert(Bean(7, 0x40, "Oscillator high gain"));
            switches.insert(Bean(7, 0x02, "External system clock source"));
            switches.insert(Bean(8, 0x20, "WDT disable after power-on-reset"));
            switches.insert(Bean(8, 0x04, "WDT count in idle mode"));
            switches.insert(Bean(10, 0x02, "Not erase data EEPROM"));
            switches.insert(Bean(10, 0x01, "Download regardless of P1"));
            echoPrint(" WDT prescal: %d\n", pow(2, ((this->info[8] & 0x07) + 1)));
        } else if (in<String, const char *>(this->protocol.val, PROTOSET_12B, ARR_SIZE(PROTOSET_12B))) {
            switches.insert(Bean(8, 0x02, "Not erase data EEPROM"));
        } else
            switches.clear();

        LinkedList<Bean>::Iterator it = switches.getIterator();
        while (it.hasNext()) {
            Bean bean = it.next();
            echoPrint(" [%c] %s\n", (this->info[bean.pos] & bean.bit) ? 'X' : ' ', bean.desc);
        }
    }

    void send(uchar cmd, const Array<uchar> &dat) const {
        SequentialList<uchar> buf;
        buf.insert(0x46).insert(0xB9).insert(0x6A);
        int n = 1 + 2 + 1 + dat.length() + this->chkmode + 1;
        buf.insert(n >> 8).insert(n & 0xFF).insert(cmd);
        buf.insert(buf.length(), dat.elements, dat.length());

        int chksum = sum<int, uchar>(buf.data, 2, buf.length());
        if (this->chkmode > 1) {
            buf.insert((chksum >> 8) & 0xFF);
        }
        buf.insert(chksum & 0xFF).insert(0x16);
        __conn_write(buf.data, buf.length());
    }

    void unknown_packet_1() const {
        if (in<String, const char *>(this->protocol.val, PROTOSET_PARITY, ARR_SIZE(PROTOSET_PARITY))) {
            logging.info("Send unknown packet (50 00 00 36 01 ...)\n");
            uchar b[] = {0x00, 0x00, 0x36, 0x01};
            send(0x50, Array<uchar>::from(b, 4));
            const Bean2<uchar, Array<uchar>> recvBean = this->recv();
            uchar cmd = recvBean.a1;
            Array<uchar> dat = recvBean.a2;
            assert(cmd == 0x8F && dat.length() == 0);
        }
    }

    void handshake() {
        uint32_t baud0 = conn.getBaud();
        uint32_t bauds[] = {115200, 57600, 38400, 28800, 19200, 14400, 9600, 4800, 2400, 1200},
                 len = ARR_SIZE(bauds);
        bool broken = false;
        uint32_t baud;
        Array<int> baudstr(0);
        for (int i = 0; i < len; ++i) {
            baud = bauds[i];
            double t = this->fosc * 1000000 / baud / 32;
            if (!in<String, const char *>(this->protocol.val, PROTOSET_89, ARR_SIZE(PROTOSET_89))) {
                t *= 2;
            }
            if (std::abs(round(t) - t) / t > 0.03) continue;
            int tcfg;
            if (in<String, const char *>(this->protocol.val, PROTOSET_89, ARR_SIZE(PROTOSET_89))) {
                tcfg = 0x10000 - (int) (t + 0.5);
            } else {
                if (t > 0xFF) continue;
                tcfg = 0xC000 + 0x100 - (int) (t + 0.5);
            }
            int baudstrT[] = {tcfg >> 8, tcfg & 0xFF, 0xFF - (tcfg >> 8), min((256 - (tcfg & 0xFF)) * 2, 0xFE),
                              (int) (baud0 / 60)};
            baudstr = Array<int>::from(baudstrT, ARR_SIZE(baudstrT));
            logging.info("Test baudrate %d (accuracy %0.4f) using config %s\n", baud, std::abs(round(t) - t) / t,
                         hexStrJoin<int>(' ', baudstr).getCString());
            Array<int> freqlist(0);
            if (in<String, const char *>(this->protocol.val, PROTOSET_89, ARR_SIZE(PROTOSET_89))) {
                int a[] = {40, 20, 10, 5};
                freqlist = Array<int>::from(a, ARR_SIZE(a));
            } else {
                int a[] = {30, 24, 20, 12, 6, 3, 2, 1};
                freqlist = Array<int>::from(a, ARR_SIZE(a));
            }
            int twait;
            for (twait = 0; twait < freqlist.length(); ++twait) {
                if (this->fosc > freqlist[twait]) {
                    broken = true;
                    break;
                };
            }
            logging.info("Waiting time config %02X\n", (0x80 + twait));
            Array<uchar> b(baudstr.length() + 1);
            for (int j = 0; j < b.length() - 1; ++j) {
                b[j] = (uchar) baudstr[j];
            }
            b[b.length() - 1] = 0x80 + twait;
            send(0x8F, b);
            try {
                __conn_baudrate(baud);
                const Bean2<uchar, Array<uchar>> rb = recv();
                uchar cmd = rb.a1;
                Array<uchar> dat = rb.a2;
                __conn_baudrate(baud0, false);
                broken = true;
                break;
            } catch (...) {
                logging.info("Cannot use baudrate %d\n", baud);
                Thread::sleep(200);
                // TODO omit: `flushInput()`
                __conn_baudrate(baud0, false);
            }
        }
        if (!broken) throw String("io error");
        logging.info("Change baudrate to %d\n", baud);
        Array<uchar> b2(baudstr.length());
        for (int i = 0; i < b2.length(); ++i) {
            b2[i] = (uchar) baudstr[i];
        }
        send(0x8E, b2);
        __conn_baudrate(baud);
        conn.setSpeed(baud);
        this->bundrate = baud;
        const Bean2<uchar, Array<uchar>> rb = recv();
        uchar cmd = rb.a1;
        Array<uchar> dat = rb.a2;
    }

    void unknown_packet_2() const {
        if (!in<String, const char *>(this->protocol.val, PROTOSET_PARITY, ARR_SIZE(PROTOSET_PARITY))) {
            for (int i = 0; i < 5; ++i) {
                logging.info("Send unknown packet (80 00 00 36 01 ...)\n");
                uchar a[] = {0x00, 0x00, 0x36, 0x01};
                int aSize = (int) ARR_SIZE(a);
                Array<uchar> s(aSize + this->model.length());
                for (int j = 0; j < aSize; ++j) {
                    s[j] = a[j];
                }
                for (int j = aSize; j < s.length(); ++j) {
                    s[j] = this->model[j - aSize];
                }
                send(0x80, s);
                const Bean2<uchar, Array<uchar>> rb = recv();
                assert(rb.a1 == 0x80 && rb.a2.length() == 0);
            }
        }
    }

    void erase() const {
        if (in<String, const char *>(this->protocol.val, PROTOSET_89, ARR_SIZE(PROTOSET_89))) {
            uchar a[] = {0x01, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33};
            send(0x84, Array<uchar>::from(a, ARR_SIZE(a)));
            const Bean2<uchar, Array<uchar>> rb = recv(10);
            assert(rb.a1 == 0x80);
        } else {
            uchar a[] = {0x00, 0x00, (uchar)(this->romsize.val * 4), 0x00, 0x00, (uchar)(this->romsize.val * 4)};
            int len = ARR_SIZE(a);
            SequentialList<uchar> b(len);
            for (int i = 0; i < len; ++i) {
                b.insert(a[i]);
            }
            for (int i = 0; i < 12; ++i) {
                b.insert(0);
            }
            for (int i = 0x80; i > 0x0D; --i) {
                b.insert(i);
            }

            const Array<uchar> c = list2arr<uchar>(b);
            send(0x84, c);
            const Bean2<uchar, Array<uchar>> rb = recv(10);
            Array<uchar> dat = rb.a2;
            if (dat.length() != 0) {
                String msg = "Serial number: ";
                msg += hexStrJoin<uchar>(' ', dat);
                logging.info("%s\n", msg.getCString());
            }
        }
    }

    void flash(Code &nonableCode, Consumer<double> *yieldCallback) const {
        Array<uchar> &val = nonableCode.val;
        SequentialList<uchar> codeList(val.length());
        for (int i = 0; i < val.length(); ++i) {
            codeList.insert(val[i]);
        }
        int repeatCount = 511 - (val.length() - 1) % 512;
        for (int i = 0; i < repeatCount; ++i) {
            codeList.insert(0x00);
        }
        Array<uchar> code = list2arr<uchar>(codeList);

        for (int i = 0; i < code.length(); i += 128) {
            logging.info("Flash code region (%04X, %04X)\n", i, i + 127);

            uchar a[] = {0, 0, (uchar)(i >> 8), (uchar)(i & 0xFF), 0, 128};
            int len = ARR_SIZE(a);
            SequentialList<uchar> b(len);
            for (int j = 0; j < len; ++j) {
                b.insert(a[j]);
            }
            SequentialList<uchar> cut = cutList(codeList, i, i + 128);
            for (int j = 0; j < cut.length(); ++j) {
                b.insert(cut[j]);
            }
            const Array<uchar> &sent = list2arr<uchar>(b);
            send(0x00, sent);
            const Bean2<uchar, Array<uchar>> rb = recv();
            uchar cmd = rb.a1;
            Array<uchar> dat = rb.a2;

            int sum = 0;
            for (int j = 0; j < cut.length(); ++j) {
                sum += cut[j];
            }
            assert(dat[0] == sum % 256);
            double r = ((double) (i + 128)) / code.length();
            yieldCallback->accept(r);
        }
    }

    void unknown_packet_3() const {
        if (in<String, const char *>(this->protocol.val, PROTOSET_PARITY, ARR_SIZE(PROTOSET_PARITY))) {
            logging.info("Send unknown packet (69 00 00 36 01 ...)\n");
            uchar a[] = {0x00, 0x00, 0x36, 0x01};
            int len = ARR_SIZE(a);
            Array<uchar> sent(len + this->model.length());
            for (int i = 0; i < len; ++i) {
                sent[i] = a[i];
            }
            for (int i = len; i < sent.length(); ++i) {
                sent[i] = this->model[i - len];
            }
            send(0x69, sent);
            const Bean2<uchar, Array<uchar>> &rb = recv();
            uchar cmd = rb.a1;
            Array<uchar> dat = rb.a2;
            assert(cmd == 0x8D && dat.length() == 0);
        }
    }

    [[nodiscard]] bool options(NonableBoolean erase_eeprom) const {
        SequentialList<uchar> dat;
        const Array<uchar> localFosc = packUnsignedInt32BigEndian((uint32_t)(this->fosc * 1000000));
        if (this->protocol.val == PROTOCOL_89) {
            if (!erase_eeprom.isNone) {
                this->info[2] &= 0xF7;
                this->info[2] |= erase_eeprom.val ? 0x00 : 0x08;
            }

            auto cut = cutArray<uchar>(this->info, 2, 3);
            for (int i = 0; i < cut.length(); ++i) {
                dat.insert(cut[i]);
            }
            for (int i = 0; i < 3; ++i) {
                dat.insert(0xFF);
            }
        } else if (this->protocol.val == PROTOCOL_12C5A) {
            if (!erase_eeprom.isNone) {
                this->info[10] &= 0xFD;
                this->info[10] |= erase_eeprom.val ? 0x00 : 0x02;
            }
            insertArrToList<uchar>(dat, cutArray<uchar>(this->info, 6, 9));
            repeatListInsert<uchar>(dat, 0xFF, 5);
            insertArrToList<uchar>(dat, cutArray<uchar>(this->info, 10, 11));
            repeatListInsert<uchar>(dat, 0xFF, 6);
            insertArrToList<uchar>(dat, localFosc);
        } else if (in<String, const char *>(this->protocol.val, PROTOSET_12B, ARR_SIZE(PROTOSET_12B))) {
            if (!erase_eeprom.isNone) {
                this->info[8] &= 0xFD;
                this->info[8] |= erase_eeprom.val ? 0x00 : 0x02;
            }
            insertArrToList<uchar>(dat, cutArray(this->info, 6, 11));
            insertArrToList<uchar>(dat, localFosc);
            insertArrToList<uchar>(dat, cutArray(this->info, 12, 16));
            repeatListInsert<uchar>(dat, 0xFF, 4);
            insertArrToList<uchar>(dat, cutArray(this->info, 8, 9));
            repeatListInsert<uchar>(dat, 0xFF, 7);
            insertArrToList<uchar>(dat, localFosc);
            repeatListInsert<uchar>(dat, 0xFF, 3);
        } else if (!erase_eeprom.isNone) {
            logging.info("Modifying options is not supported for this target\n");
            return false;
        }

        if (dat.length() != 0) {
            send(0x8D, list2arr<uchar>(dat));
            const Bean2<uchar, Array<uchar>> rb = recv();
        }
        return true;
    }

    void terminate() const {
        logging.info("Send termination command\n");

        send(0x82, Array<uchar>(0));
        this->conn.flush();
        Thread::sleep(200);
    }
};

void program(Programmer &prog, Code &code, NonableBoolean erase_eeprom = NonableBoolean(true, true)) {
    echoPrint("%s", "Detecting target...");
    echo->flush();
    prog.detect();
    echoPrint(" done\n");
    prog.print_info();

    if (prog.protocol.isNone) throw String("Unsupported target");
    if (code.isNone) return;
    prog.unknown_packet_1();

    echoPrint("Baudrate: ");
    echo->flush();
    prog.handshake();
    echoPrint("%u\n", prog.bundrate);

    prog.unknown_packet_2();

    echoPrint("Erasing target...");
    echo->flush();

    prog.erase();

    echoPrint(" done\n");
    echoPrint("Size of the binary: %d\n", code.val.length());

    echoPrint("Programming: ");
    echo->flush();
    int oldbar = 0;

    class C : public Consumer<double> {
    private:
        int &oldbar;

    public:
        void accept(double &progress) override {
            int bar = (int) (progress * 20);
            for (int i = 0; i < bar - oldbar; ++i) {
                echoPrint("#");
            }
            oldbar = bar;
            echo->flush();
        }

        explicit C(int &oldbar) : oldbar(oldbar) {}
    } c(oldbar);
    prog.flash(code, &c);

    echoPrint(" done\n");

    prog.unknown_packet_3();

    echoPrint("Setting options...");
    echo->flush();

    if (prog.options(erase_eeprom)) {
        echoPrint(" done\n");
    } else {
        echoPrint(" failed\n");
    }

    prog.terminate();
}

int bczhc::run(const String &hexFile, EchoCallback *echoCallback, serial::Serial *serialImpl) {
    echo = echoCallback;
    struct Opt {
        uint32_t aispbaud = 4800;
        NonableString aispmagic = NonableString(nullptr, true);
        NonableBoolean erase_eeprom = NonableBoolean(false);
        InputStream *image = nullptr;
        uint32_t lowbaud = 2400;
        bool not_erase_eeprom = false;
        String port = nullptr;
        NonableString protocol = NonableString("auto");
        int verbose = 0;
        int loglevel = 0;
    } opts;

    opts.port = serialImpl->getPortName();

    const String &filename = hexFile;
    opts.image = new InputStream(filename);

    opts.loglevel = Logging::combination[min(2, opts.verbose)];
    opts.protocol = chooseProtocol(opts.protocol.val.getCString());

    if (!opts.erase_eeprom.val && !opts.not_erase_eeprom) {
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
        code.val = localCode.val;
    } else
        code.isNone = true;
    echoPrint("Connect to %s at baudrate %d\n", opts.port.getCString(), opts.lowbaud);
    serial::Serial &conn = *serialImpl;
    conn.setSpeed(opts.lowbaud);
    if (!opts.aispmagic.isNone) autoisp(conn, opts.aispbaud, opts.aispmagic);
    Programmer programmer(conn, opts.protocol);
    program(programmer, code, opts.erase_eeprom);
    return 0;
}
