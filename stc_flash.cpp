#include "./string.hpp"

using namespace bczhc;
using namespace string;

static char *port;

void judgeOS() {
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
    }
}

int main(int argc, char **argv) {
    return 0;
}
