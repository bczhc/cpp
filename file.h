//
// Created by zhc on 11/21/20.
//

#ifndef BCZHC_FILE_H
#define BCZHC_FILE_H

#include "./string.hpp"
#include "string.hpp"
#include <cstdint>
#include <cstdio>

using namespace bczhc;

namespace bczhc {
    class File {
    private:
        String path;
    public:
#ifdef _WIN32
        constexpr static char separator = '\\';
#else
        constexpr static char separator = '/';
#endif

        explicit File(const char *path);

        int remove();

        static int move(const char *fileName);

        int64_t getFileSize();

        static int64_t getFileSize(FILE *fp);

        static String getExtension(const String &path);

        static String getFileName(const String &path);
    };
}

#endif //BCZHC_FILE_H
