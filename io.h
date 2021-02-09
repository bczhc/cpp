#ifndef BCZHC_IO_H
#define BCZHC_IO_H

#include "string.hpp"
#include <cstdio>

#ifndef BCZHC_IO_BUFFER_SIZE
#define BCZHC_IO_BUFFER_SIZE 8192
#endif

using namespace bczhc;

namespace bczhc {
    class U8StringCallback {
    public:
        virtual void callback(char *s, int size) = 0;
    };

    void solveU8FromStream(FILE *f, U8StringCallback &callback);

    class InputStream {
    private:
        FILE *fp = nullptr;
        bool closed = false;

    public:
        explicit InputStream(const String& file);

        explicit InputStream(FILE *stream);

        ~InputStream();

        int read(char *bytes, int size);

        void close();
    };

    class OutputStream {
    private:
        FILE *fp = nullptr;
        bool closed = false;

    public:
        explicit OutputStream(const String& file);

        explicit OutputStream(FILE *stream);

        ~OutputStream();

        int write(const char *bytes, int size);

        void flush();

        void close();
    };

    class
    LineReader {
    private:
        InputStream &is;

    public:
        explicit LineReader(InputStream &in);

        String readLine();
    };
}// namespace bczhc
#endif// BCZHC_IO_H
