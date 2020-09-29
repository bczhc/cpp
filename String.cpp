#include "String.h"
#include "./utf8.h"
#include <cstdio>
#include <cstring>

using namespace bczhc;
using namespace string;
using namespace utf8;

String::String() {
}

String::String(const String &string) {
    copy(string);
}

void String::copy(const String &string) {
    data = string.data;
    dataSize = string.dataSize;
    stringSize = string.stringSize;
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
    int c = 0;
    int i = 0;
    while (data[i] != '\0') {
        i += getUTF8BytesLength(data[i]);
        ++c;
    }
    return c;
}

int String::size() {
    return stringSize;
}

void String::fromCharsString(const char *s) {
    stringSize = strlen(s);
    dataSize = stringSize + 1;
    data = new char[dataSize];
    strcpy(data, s), data[stringSize] = '\0';
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
    if (stringSize + len + 1 > dataSize) {
        dataSize = 2 * (stringSize + len) + 1;
        resize(dataSize);
    }
    for (int i = 0; i < len; ++i)
        data[stringSize + i] = s[i];
    stringSize += len;
}

void String::append(const String &string) {
    append(string.data);
}

void String::append(char c) {
    if (stringSize + c + 1 > dataSize) {
        dataSize = 2 * stringSize;
        resize(dataSize);
    }
    data[stringSize] = c;
    ++stringSize;
}

int String::indexOf(char c) {
    for (int i = 0; i < stringSize; ++i)
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
