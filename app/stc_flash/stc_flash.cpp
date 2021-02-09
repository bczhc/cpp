//
// Created by zhc on 1/31/21.
//

#include <iostream>
#include "stc_flash_lib.h"
#include "serial_linux.h"

using namespace std;
using namespace bczhc;

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

int main(int argc, char **argv) {
    String name = File::getFileName(argv[0]);
    if (argc != 2 || (String::equal(argv[1], "--help") || String::equal(argv[1], "-h"))) {
        cout << "Usage: " << name.getCString() << " <hex-file-path>" << endl;
        return 0;
    }

    class CB : public EchoCallback {
    public:
        void print(const char *s) override {
            cout << s;
        }

        void flush() override {
            cout.flush();
        }
    } callback;

    int status = 1;
    try {
        SerialLinux serialImpl(getPort().getCString());
        status = run(argv[1], &callback, &serialImpl);
    } catch (const String &e) {
        cerr << e.getCString() << endl;
        return status;
    }
    return status;
}
