#include "../io.h"
#include <bits/types/FILE.h>
#include <csignal>
#include <cstdio>
#include <iostream>

using namespace std;
using namespace bczhc::io;

static bool r = true;

void sigHandler(int sig) {
    if (sig == SIGINT) {
        r = false;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sigHandler);
    try {
        OutputStream os(argv[1]);
        char c;
        while (r) {
            fwrite(&c, 1, 3, stdout);
            printf("%c ", c);
            //fread(&buf, 1, 1, stdin);
            os.write(&c, 1);
            os.flush();
        }
    } catch (String s) {
        cout << s.getCString() << endl;
        return 1;
    }
    return 0;
}
