//
// Created by zhc on 11/21/20.
//

#ifndef CPP_FILE_H
#define CPP_FILE_H

#include "./String.h"
#include <cstdint>
#include <cstdio>

using namespace bczhc;
using namespace string;

namespace bczhc::file {
    class File {
    private:
        String path;
    public:
        explicit File(const char *path);

        int remove();

        int64_t getFileSize() {
            FILE *fp = fopen(path.getCString(), "rb");
            int64_t sz = File::getFileSize(fp);
            fclose(fp);
            return sz;
        }

        static int64_t getFileSize(FILE *fp) {
            int64_t sz;
            fseek(fp, 0L, SEEK_END);
            sz = (int64_t) ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            return sz;
        }
    };
}

#endif //CPP_FILE_H
