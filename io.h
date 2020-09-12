#ifndef BCZHC_IO_H
#define BCZHC_IO_H

#include "utf8.h"
#include <cstdio>

#define BUFFER_SIZE 8192

using namespace bczhc::utf8;

namespace bczhc {
namespace io {

class U8StringCallback {
public:
    virtual void callback(char *s, int size) = 0;
};

void solveU8FromStream(FILE *f, U8StringCallback &callback);
} // namespace io
} // namespace bczhc
#endif // BCZHC_IO_H
