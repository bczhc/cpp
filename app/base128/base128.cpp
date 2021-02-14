//
// Created by zhc on 11/21/20.
//

#include "../../io.hpp"
#include "../base128/Base128Lib.h"

using namespace bczhc;

static int bufferSize = 7168; //7 * 1024

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    int outBufferSize = bufferSize / 7 * 8;

    InputStream in(stdin);
    OutputStream out(stdout);

    uchar inBuf[bufferSize];
    uchar outBuf[outBufferSize];
    int read;
    while ((read = in.read((char *) inBuf, bufferSize)) > 0) {
        int times = read / 7;
        int left = read % 7;
        int i = 0;
        for (; i < times; ++i) {
            encode7bytes(outBuf + i * 8, inBuf + i * 7);
        }
        if (left != 0) {
            memset(inBuf + i * 7 + left, 0, 7 - left);
            encode7bytes(outBuf + i * 8, inBuf + i * 7);
            ++i;
        }
        out.write((char *) outBuf, i * 8);
    }

    in.close();
    out.close();
    return 0;
}
