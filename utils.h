//
// Created by zhc on 11/21/20.
//

#ifndef CPP_UTILS_H
#define CPP_UTILS_H

#include <cstdint>
#include <sys/time.h> // NOLINT(modernize-deprecated-headers)

namespace bczhc::utils {
    int64_t getCurrentTimeMillis();
}

#endif //CPP_UTILS_H
