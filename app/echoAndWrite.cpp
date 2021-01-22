#include "../io.h"
#include <csignal>
#include <cstdio>
#include <iostream>

using namespace std;
using namespace bczhc::io;

static OutputStream *os;

void sigHandler(int sig) {
    if (sig == SIGINT) {
        os->flush(), os->close();
        delete os;
        exit(0);
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sigHandler);
    InputStream in(stdin);
    OutputStream out(stdout);
    try {
        os = new OutputStream(argv[1]);
        char c;
        while (in.read(&c, 1) > 0) {
            out.write(&c, 1);
            out.flush();

            os->write(&c, 1);
            os->flush();
        }
    } catch (String s) {
        cout << s.getCString() << endl;
        return 1;
    }
    return 0;
}
