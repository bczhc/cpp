//
// Created by bczhc on 2/7/21.
//

#include "test.h"
#include "../string.hpp"

using namespace bczhc;

String f1() {
    return String("s3") += '.';
}

int test() {
    String s1;
    s1 += "a";
    s1 += String().append('b') += "c2";
    if (!s1.equals("abc2")) return 1;

    String s2 = "a";
    for (int i = 0; i < 5; ++i) {
        s2.insert(0, String::toString(i));
    }
    if (!s2.equals("43210a")) return 2;

    if (!f1().equals("s3.")) return 3;

    String s4(100);
    s4 = "abc";
    String s5 = s4;
    if (s4 != s5 || !s4.equals("abc")) return 4;
    return 0;
}