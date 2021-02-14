#ifndef BCZHC_IO_HPP
#define BCZHC_IO_HPP

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
        bool closed = false;

    public:
        FILE *fp = nullptr;

        explicit InputStream(const String& file);

        explicit InputStream(FILE *stream);

        int read(char *bytes, int size);

        void close();

        InputStream &operator=(const InputStream &a);

        InputStream(const InputStream &a);
    };

    class OutputStream {
    private:
        bool closed = false;

    public:
        FILE *fp = nullptr;

        explicit OutputStream(const String& file);

        explicit OutputStream(FILE *stream);

        int write(const char *bytes, int size);

        void flush();

        void close();

        OutputStream &operator=(const OutputStream &a);

        OutputStream(const OutputStream &a);
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
#endif// BCZHC_IO_HPP
