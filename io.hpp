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
    public:
        FILE *fp = nullptr;

        InputStream();

        explicit InputStream(const String &file);

        explicit InputStream(FILE *stream);

        int read(char *bytes, int size) const;

        void close() const;

        InputStream &operator=(const InputStream &a);

        InputStream(const InputStream &a);
    };

    class OutputStream {
    public:
        FILE *fp = nullptr;

        OutputStream();

        explicit OutputStream(const String &file);

        explicit OutputStream(FILE *stream);

        int write(const char *bytes, int size) const;

        void flush() const;

        void close() const;

        OutputStream &operator=(const OutputStream &a);

        OutputStream(const OutputStream &a);
    };

    class LineReader {
    private:
        InputStream is;

    public:
        LineReader();

        explicit LineReader(const InputStream& in);

        String readLine() const;

        LineReader(const LineReader &a);

        LineReader &operator=(const LineReader &a);
    };

    class Scanner {
    private:
        LineReader reader;
    public:
        explicit Scanner(FILE *in);

        String nextString() const;

        int32_t nextInt() const;

        int64_t nextLong() const;

        float nextFloat() const;

        double nextDouble() const;

        bool nextBool() const;
    };
}// namespace bczhc
#endif// BCZHC_IO_HPP
