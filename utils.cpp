//
// Created by zhc on 11/21/20.
//

#include "utils.h"

int64_t bczhc::utils::getCurrentTimeMillis() {
    timeval t{};
    gettimeofday(&t, nullptr);
    return t.tv_sec * 1000 + t.tv_usec / 1000;
}