#include "String.h"
#include "./utf8.h"
#include "third_party/practice/LinearList.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sys/types.h>

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
    fromCharsString(s, strlen(s));
}

String::String(const char *s, int size) {
    fromCharsString(s, size);
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

void String::fromCharsString(const char *s, int size) {
    stringSize = size, dataSize = stringSize;
    data = new char[dataSize];
    strcpy(data, s), data[size] = '\0';
}

String &String::operator=(const char *s) {
    fromCharsString(s, strlen(s));
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

SequentialList<String> String::split(const String &separator) {
    return split(separator.data);
}

SequentialList<String> String::split(const char *separator) {
    SequentialList<String> list;
    SequentialList<int> indexes;
    int length = strlen(separator);
    char *found;
    char *start = data;
    while (start - data <= stringSize && (found = strstr(start, separator)) != nullptr) {
        indexes.insert((int) (found - data));
        start = found + 1;
    }
    //abcd
    // 1
    int len = indexes.length();
    int subStart = 0;

    for (int i = 0; i < len; ++i) {
        int index = indexes.get(i);
        int size = index - subStart;
        String s(data + subStart, size);
        list.insert(s);
        subStart = index + length;
    }
    String rest(data + subStart, stringSize - subStart);
    list.insert(rest);
    return list;
}
