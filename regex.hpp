//
// Created by zhc on 12/26/20.
//

#ifndef BCZHC_REGEX_HPP
#define BCZHC_REGEX_HPP

#ifdef CMAKE_PCRE_FOUND

#include "utils.hpp"
#include "./linked_list.hpp"
#include "string.hpp"
#include <pcre.h>

using namespace bczhc;

namespace bczhc {
    using ResultList = LinkedList<String>;

    pcre *getCompiledPcre(const char *pattern);

    ResultList match(const char *pattern, const char *text);

    ResultList match(const char *pattern, const char *text, int textSize);

    bool test(const char *pattern, const char *text);

    bool test(const char *pattern, const char *text, int textSize);
}
#endif //CMAKE_PCRE_FOUND

#endif //BCZHC_REGEX_HPP