#include "String.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <sched.h>

using namespace bczhc;
using namespace string;

String::String() {
}

String::String(const String &string) {
    copy(string);
}

void String::copy(const String &string) {
    data = string.data;
    dataLength = string.dataLength;
    stringLength = string.stringLength;
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
    return stringLength;
}

void String::fromCharsString(const char *s) {
    stringLength = strlen(s);
    dataLength = stringLength + 1;
    data = new char[dataLength];
    strcpy(data, s), data[stringLength] = '\0';
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
    char *newChars = new char[newLength + 1];
    newChars[newLength] = '\0';
    strcpy(newChars, data);
    delete[] data;
    data = newChars;
}

void String::append(const char *s) {
    int len = strlen(s);
    if (stringLength + len + 1 > dataLength) {
        dataLength = 2 * (stringLength + len) + 1;
        resize(dataLength);
    }
    for (int i = 0; i < len; ++i)
        data[stringLength + i] = s[i];
    stringLength += len;
}

void String::append(const String &string) {
    append(string.data);
}

void String::append(char c) {
    if (stringLength + c + 1 > dataLength) {
        dataLength = 2 * stringLength;
        resize(dataLength);
    }
    data[stringLength] = c;
    ++stringLength;
}

int String::indexOf(char c) {
    for (int i = 0; i < stringLength; ++i)
        if (data[i] == c) return i;
    return -1;
}

int String::indexOf(const char *s) {
    char *match = strstr(data, s);
    return match == nullptr ? -1 : (int) (match - data);
}

int String::indexOf(const String &string) {
    return indexOf(string.data);
}
