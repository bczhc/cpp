#include "String.h"
#include "./utf8.h"
#include "third_party/practice/LinearList.hpp"
#include <cstddef>
#include <cstring>

using namespace bczhc;
using namespace string;
using namespace utf8;

String::String() {
    fromCharsString(nullptr, 0);
}

String::String(const String &string) {
    copy(string);
}

void String::copy(const String &string) {
    size_t oldStringSize = string.size();
    data = new char[oldStringSize + 1];
    for (int i = 0; i < oldStringSize; ++i) data[i] = string.data[i];
    data[oldStringSize] = '\0';
    stringSize = string.stringSize;
    dataSize = oldStringSize + 1;
}

String::String(const char *s) {
    fromCharsString(s, s == nullptr ? 0 : strlen(s));
}

String::String(const char *s, size_t size) {
    fromCharsString(s, size);
}

const char *String::getCString() const {
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

size_t String::size() const {
    return stringSize;
}

void String::fromCharsString(const char *s, size_t size) {
    stringSize = size, dataSize = stringSize + 1;
    data = new char[dataSize];
    for (int i = 0; i < size; ++i) data[i] = s[i];
    data[size] = '\0';
}

String &String::operator=(const char *s) {
    fromCharsString(s, s == nullptr ? 0 : strlen(s));
    return *this;
}


String &String::operator=(const String &string) {
    if (&string == this) return *this;
    copy(string);
    return *this;
}

void String::resize(int newSize) {
    char *newChars = new char[newSize];
    strcpy(newChars, data);
    delete[] data;
    data = newChars;
}

String &String::append(const char *s) {
    int len = strlen(s);
    if (stringSize + len + 1 > dataSize) {
        dataSize = 2 * (stringSize + len) + 1;
        resize(dataSize);
    }
    for (int i = 0; i < len; ++i)
        data[stringSize + i] = s[i];
    stringSize += len;
    data[stringSize] = '\0';
    return *this;
}

String &String::append(const String &string) {
    append(string.data);
    return *this;
}

String &String::append(char c) {
    if (stringSize + c + 1 > dataSize) {
        dataSize = 2 * stringSize + 1;
        resize(dataSize);
    }
    data[stringSize] = c;
    ++stringSize;
    data[stringSize] = '\0';
    return *this;
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

String &String::append(const std::string &s) {
    return append(s.c_str());
}

String &String::operator=(const std::string &str) {
    operator=(str.c_str());
    return *this;
}

String::String(const std::string &str) {
    const char *s = str.c_str();
    fromCharsString(s, strlen(s));
}

String::~String() {
    delete data;
}

String String::toString(int32_t a) {
    String s;
    int x = a;
    while (x != 0) {
        s.append((char) (x % 10 + 48));
        x /= 10;
    }
    return s;
}

String String::toString(int64_t a) {
    String s;
    int64_t x = a;
    while (x != 0) {
        s.append((char) (x % 10 + 48));
        x /= 10;
    }
    return s;
}

String String::toString(float a) {
    return String();
}

String String::toString(double a) {
    return String();
}
