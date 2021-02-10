#include "io.h"
#include "string.hpp"
#include <cstdio>
#include "./exception.hpp"

using namespace bczhc;

void bczhc::solveU8FromStream(FILE *f, U8StringCallback &callback) {
    char buf[BCZHC_IO_BUFFER_SIZE];
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
        if ((readLen = fread(buf + readOff, 1, BCZHC_IO_BUFFER_SIZE - readOff, f)) <= 0)
            break;
        int u8BytesLen = 0;
        for (int i = BCZHC_IO_BUFFER_SIZE - 1; i >= 0; --i) {
            if ((u8BytesLen = getUTF8BytesLength(buf[i])) != 0) {
                // find the last index of the first byte in valid utf8 bytes
                lastValidPos = i;
                break;
            }
        }
        if (readOff + readLen - lastValidPos == u8BytesLen)
            lastValidPos += u8BytesLen;
        callback.callback(
                buf, readLen < (BCZHC_IO_BUFFER_SIZE - readOff) ? readLen : lastValidPos);
    }
}

InputStream::InputStream(const String& file) {
    if ((fp = fopen(file.getCString(), "rb")) == nullptr) throw IOException("Open file failed.");
}

int InputStream::read(char *bytes, int size) {
    return fread(bytes, 1, size, fp);
}

void InputStream::close() {
    closed = true;
    fclose(fp);
}

OutputStream::OutputStream(const String& file) {
    if ((fp = fopen(file.getCString(), "wb")) == nullptr) throw IOException("Open file failed.");
}

int OutputStream::write(const char *bytes, int size) {
    return fwrite(bytes, 1, size, fp);
}

void OutputStream::flush() {
    fflush(fp);
}

void OutputStream::close() {
    closed = true;
    fclose(fp);
}

InputStream::InputStream(FILE *stream) {
    fp = stream;
}

InputStream::InputStream(const InputStream &a) {
    operator=(a);
}

InputStream &InputStream::operator=(const InputStream &a) {
    this->fp = a.fp;
    this->closed = a.closed;
    return *this;
}

OutputStream::OutputStream(FILE *stream) {
    fp = stream;
}

OutputStream::OutputStream(const OutputStream &a) {
    operator=(a);
}

OutputStream &OutputStream::operator=(const OutputStream &a) {
    this->fp = a.fp;
    this->closed = a.closed;
    return *this;
}

LineReader::LineReader(InputStream &in) : is(in) {}

String LineReader::readLine() {
    String line;
    char c;
    while (is.read(&c, 1) > 0) {
        if (c == '\n') {
            return line;
        }
        line.append(c);
    }
    return nullptr;
}
