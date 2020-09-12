#include "CountCharacters.h"
#include "utf8.h"
#include <cstdio>

#define BUFFER_SIZE 8192
using namespace bczhc;
using namespace utf8;

int main() {
    CharacterCounter counter;
    FILE *in = stdin;
    char buf[BUFFER_SIZE];
    int readLen = 0;
    int lastValidPos = 0;
    int readOff = 0;
    while (true) {
        // copy the rest bytes last time to buffer
        for (int i = lastValidPos; i < readLen + readOff; ++i) {
            buf[i - lastValidPos] = buf[i];
        }
        // offset of reading next time
        if (lastValidPos != 0)
            readOff = readLen + readOff - lastValidPos;
        if ((readLen = fread(buf + readOff, 1, BUFFER_SIZE - readOff, in)) <= 0)
            break;
        if (readLen < BUFFER_SIZE - readOff)
            continue;
            int u8BytesLen = 0;
        for (int i = BUFFER_SIZE - 1; i >= 0; --i) {
            if ((u8BytesLen = getUTF8BytesLength(buf[i])) != 0) {
                // find the last index of the first byte in valid utf8 bytes
                lastValidPos = i;
                break;
            }
        }
        if (readOff + readLen - lastValidPos == u8BytesLen) lastValidPos += u8BytesLen;
        counter.countCharacters(buf, lastValidPos);
    }
    json *j = counter.getJsonData();
    printf("%s\n", j->dump().c_str());
    delete j;
    return 0;
}
