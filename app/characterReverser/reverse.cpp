#include "../../string.hpp"
#include <cstdio>

using namespace bczhc;
using namespace std;

#define BCZHC_IO_BUFFER_SIZE 4096

int main() {
    String s = "";
    char buf[BCZHC_IO_BUFFER_SIZE];
    int readLen;
    while ((readLen = fread(buf, 1, BCZHC_IO_BUFFER_SIZE, stdin)) > 0) {
        for (int i = 0; i < readLen; ++i)
            s.append(buf[i]);
    }
    const char *sp = s.getCString();
    int len = s.length();
    int bytesLength;
    for (int i = len - 1; i >= 0; --i) {
        if ((bytesLength = getUTF8BytesLength(sp[i])) != -1) {
            for (int j = 0; j < bytesLength; ++j)
                printf("%c", sp[i + j]);
        }
    }
    return 0;
}
