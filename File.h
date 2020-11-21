//
// Created by zhc on 11/21/20.
//

#ifndef CPP_FILE_H
#define CPP_FILE_H

#include "./String.h"
#include <cstdint>

using namespace bczhc;
using namespace string;

namespace bczhc::file {
    class File {
    private:
        String path;
    public:
        explicit File(const char *path);

        int remove();
    };
}

#endif //CPP_FILE_H