#include "./string.hpp"
#include "file.h"
#include "io.h"
#include "utils.hpp"
#include "array.hpp"
//#include "valgrind/valgrind.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;
using namespace bczhc::array;

static const char *PROTOCOL_89 = "89";
static const char *PROTOCOL_12C5A = "12c5a";
static const char *PROTOCOL_12C52 = "12c52";
static const char *PROTOCOL_12Cx052 = "12cx052";
static const char *PROTOSET_89[] = {PROTOCOL_89};
static const char *PROTOSET_12[] = {PROTOCOL_12C5A, PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_12B[] = {PROTOCOL_12C52, PROTOCOL_12Cx052};
static const char *PROTOSET_PARITY[] = {PROTOCOL_12C5A, PROTOCOL_12C52};

#define COMPARE_CODE0 0;

void getPort(String &port) {
    String platform;
#ifdef _WIN32
    platform = "win32";
#endif
#ifdef __APPLE__
    platform = "darwin";
#endif

    if (platform.equals("win32")) {
        port = "COM3";
    } else if (platform.equals("darwin")) {
        port = "/dev/tty.usbserial";
    } else port = "/dev/ttyUSB0";
}

void m() {
}

class Logging {
public:
    inline static int CRITICAL = 50;
    inline static int INFO = 20;
    inline static int DEBUG = 10;

    inline static int combination[] = {CRITICAL, INFO, DEBUG};
};

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

int main(int argc, char **argv) {
    OutputStream os = OutputStream("/dev/ttyUSB0");
    char s[] = "hello";
    os.write(s, 5);
    os.close();
    return 0;
    struct Opt {
        int aispbaud = 4800;
        void *aispmagic = nullptr;
        NonableBoolean erase_eeprom = NonableBoolean(false);
        InputStream *image = nullptr;
        int lowbaud = 2400;
        bool not_erase_eeprom = false;
        String port = "/dev/ttyUSB0";
        NonableString protocol = NonableString("auto");
        int verbose = 0;
        int loglevel = 0;
    } opts;

    getPort(opts.port);

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


    return 0;
}

#pragma clang diagnostic pop
