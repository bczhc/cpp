//
// Created by zhc on 12/26/20.
//

#ifndef CPP_REGEXP_H
#define CPP_REGEXP_H

#include "utils.hpp"
#include "./third_party/practice/LinearList.hpp"
#include "String.h"
#include <pcre.h>

using namespace bczhc;
using namespace utils;
using namespace linearlist;
using namespace string;

namespace bczhc::regex {
    using ResultList = LinkedList<String>;

    pcre *getCompiledPcre(const char *pattern);

    ResultList match(const char *pattern, const char *text);

    ResultList match(const char *pattern, const char *text, int textSize);

    bool test(const char *pattern, const char *text);

    bool test(const char *pattern, const char *text, int textSize);
}

#endif //CPP_REGEXP_H
