#include "String.h"
#include <algorithm>
#include <cstdio>
#include <sched.h>

using namespace bczhc;
using namespace string;

String::String(const String &string) {
    copy(string);
}

void String::copy(const String &string) {
    data = string.data;
    len = string.len;
    *string.toModify = true;
}

String::~String() {
    if (!willBeCopied) delete data;
}

String::String(const char *s) {
    fromCharsString(s);
}

const char *String::getCString() {
    return data;
}

std::string String::getCppString() {
    std::string s = data;
    return s;
}

int String::length() {
    return len;
}

void String::fromCharsString(const char *s) {
    len = strlen(s);
    data = new char[len + 1];
    strcpy(data, s), data[len] = '\0';
}

String &String::operator=(const char *s) {
    fromCharsString(s);
    return *this;
}


String &String::operator=(const String &string) {
    copy(string);
    return *this;
}

void String::resize(int newLength) {
}

void String::append(const char *s) {
}

void String::append(char *c) {
}

int String::indexOf(char *c) {
    return -1;
}

int String::indexOf(const char *s) {
    return -1;
}
