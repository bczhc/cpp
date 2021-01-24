//
// Created by zhc on 11/21/20.
//

#ifndef CPP_UTILS_HPP
#define CPP_UTILS_HPP

#include <cstdint>
#include <sys/time.h> // NOLINT(modernize-deprecated-headers)
#include <cstdio>
#include "./string.hpp"

using namespace bczhc::string;

namespace bczhc::utils {
    template<typename T>
    class Result {
        int status = 0;
        T r;
    };

    int64_t getCurrentTimeMillis();

    int min(int a, int b);

    int parseInt(const String &s, int radix);
}

#endif //CPP_UTILS_HPP
