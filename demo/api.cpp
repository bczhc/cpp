//
// Created by zhc on 2/5/21.
//

#include "api.h"
#include "../string.hpp"

using namespace bczhc::string;

void toString(char **dst, int i) {
    String s = String::toString(i);
    *dst = (char *) malloc(s.length() + 1);
    strcpy(*dst, s.getCString());
}
