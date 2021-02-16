//
// Created by bczhc on 2/7/21.
//

#include "test.h"
#include "../string.hpp"

using namespace bczhc;

String f1() {
    return String("s3") += '.';
}

String f2(String a) {
    auto b = a;
    auto c = b;
    c = a;
    b = a;
    a = b;
    auto d = a, e = b;
    d = e;
    a = b;
    a = d, e = a;
    e = f1();
    return a + b + c + d + e;
}

int test1() {
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
    if (!f2("a").equals("aaaas3.")) return 5;
    return 0;
}