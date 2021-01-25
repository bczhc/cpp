#include "./string.hpp"
#include "file.h"
#include "io.h"
#include "utils.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
using namespace bczhc;
using namespace string;
using namespace file;
using namespace io;
using namespace utils;

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

using NonableString = TypeWithNone<String>;
using NonableBoolean = TypeWithNone<bool>;

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
char *readToBytes(InputStream &in) {
    SequentialList<char> b;
    char buf[4096];
    int readLen;
    while ((readLen = in.read(buf, 4096)) > 0) {
        for (int i = 0; i < readLen; ++i) {
            b.insert(buf[i]);
        }
    }
    int size = b.length();
    char *r = new char[size];
    for (int i = 0; i < size; ++i) {
        r[i] = b.data[i];
    }
    return r;
}

void hex2bytes(const String &hexString) {

}

void hex2bin(const char *code) {
    SequentialList<char> buf;
    int base = 0, line = 0;

    SequentialList<String> lines;
    String lineStr = String();
    for (int j = 0; code[j] != '\0'; ++j) {
        char c = code[j];
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
        String sub1 = rec.substring(1, 3);
        char n = (char) parseInt(sub1, 16); //TODO
        String sub2 = rec.substring(1, n * 2 + 11);

    }
}

int main(int argc, char **argv) {
    try {
        int i = parseInt("123", 10);
        printf("%i\n", i);
    } catch (const String& e) {
        printf("%s\n", e.getCString());
    }

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

        char *code = readToBytes(*opts.image);
        opts.image->close();
        delete opts.image;
        String extension = File::getExtension(filename);
        if (extension.equals("hex") || extension.equals("ihx")) {
            hex2bin(code);
        }

    }
    return 0;
}

#pragma clang diagnostic pop
