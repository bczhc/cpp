#ifndef BCZHC_IO_H
#define BCZHC_IO_H

#include "String.h"
#include <cstdio>

#define BUFFER_SIZE 8192

using namespace bczhc;
using namespace string;
using namespace utf8;

namespace bczhc {
    namespace io {

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
            InputStream(String file);

            InputStream(FILE *stream);

            ~InputStream();

            int read(char *bytes, int size);

            void close();
        };

        class OutputStream {
        private:
            FILE *fp = nullptr;
            bool closed = false;

        public:
            OutputStream(String file);

            OutputStream(FILE *stream);

            ~OutputStream();

            int write(const char *bytes, int size);

            void flush();

            void close();
        };

        class LineReader {
        private:
            InputStream &is;

        public:
            LineReader(InputStream &in);

            String readLine();
        };
    }// namespace io
}// namespace bczhc
#endif// BCZHC_IO_H
