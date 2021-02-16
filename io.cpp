#include "io.hpp"
#include "string.hpp"
#include <cstdio>
#include "./exception.hpp"
#include "./utils.hpp"
#include "./utf8.hpp"

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

InputStream::InputStream(const String &file) {
    if ((fp = fopen(file.getCString(), "rb")) == nullptr) throw IOException("Open file failed.");
}

int InputStream::read(char *bytes, int size) const {
    return fread(bytes, 1, size, fp);
}

void InputStream::close() const {
    fclose(fp);
}

OutputStream::OutputStream(const String &file) {
    if ((fp = fopen(file.getCString(), "wb")) == nullptr) throw IOException("Open file failed.");
}

int OutputStream::write(const char *bytes, int size) const {
    return fwrite(bytes, 1, size, fp);
}

void OutputStream::flush() const {
    fflush(fp);
}

void OutputStream::close() const {
    fclose(fp);
}

InputStream::InputStream(FILE *stream) {
    fp = stream;
}

InputStream::InputStream(const InputStream &a) {
    operator=(a);
}

InputStream::InputStream() = default;

InputStream &InputStream::operator=(const InputStream &a) = default;

OutputStream::OutputStream(FILE *stream) {
    fp = stream;
}

OutputStream::OutputStream(const OutputStream &a) {
    operator=(a);
}

OutputStream::OutputStream() = default;

OutputStream &OutputStream::operator=(const OutputStream &a) = default;

LineReader::LineReader(const InputStream& in) : is(in) {}

String LineReader::readLine() const {
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

LineReader &LineReader::operator=(const LineReader &a) {
    if (&a == this) return *this;
    this->is = a.is;
    return *this;
}

LineReader::LineReader() = default;

LineReader::LineReader(const LineReader &a) = default;

Scanner::Scanner(FILE *in) {
    auto is = InputStream(in);
    reader = LineReader(is);
}

String Scanner::nextString() const {
    const String line = reader.readLine();
    if (line.isNull()) throw IOException("io error");
    return line;
}

int32_t Scanner::nextInt() const {
    return Integer::parseInt(nextString());
}

int64_t Scanner::nextLong() const {
    return Long::parseLong(nextString());
}

bool Scanner::nextBool() const {
    auto r = nextString();
    try {
        auto i = Integer::parseInt(r);
        return i != 0;
    } catch (const NumberFormatException &ignored) {
    }
    return String::toLowerCase(r).equals("true");
}

float Scanner::nextFloat() const {
    // TODO
    return 0;
}

double Scanner::nextDouble() const {
    // TODO
    return 0;
}
