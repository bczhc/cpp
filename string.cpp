#include "string.hpp"
#include "third_party/practice/LinearList.hpp"
#include <cstddef>
#include <cstring>
#include "utils.hpp"

using namespace bczhc;
using namespace string;
using namespace utf8;

String::String() {
    fromCharsString(nullptr, 0);
}

String::String(const String &string) {
    copy(string);
    ++*deleteCount;
}

void String::copy(const String &string) {
    deleteCount = string.deleteCount;
    data = string.data;
    dataSize = string.dataSize;
    stringSize = string.stringSize;
    mIsNull = string.mIsNull;
}

String::String(const char *s) {
    if (s == nullptr) {
        mIsNull = true;
        s = "(null)";
    }
    fromCharsString(s, strlen(s));
}

String::String(const char *s, size_t size) {
    fromCharsString(s, size);
}

const char *String::getCString() const {
    return data;
}

size_t String::utf8Length() {
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

void String::newData(const char *s, size_t size) {
    stringSize = size, dataSize = stringSize + 1;
    data = new char[dataSize];
    for (int i = 0; i < size; ++i) data[i] = s[i];
    data[size] = '\0';
}

void String::fromCharsString(const char *s, size_t size) {
    release();
    deleteCount = new int(0);
    newData(s, size);
}


String &String::operator=(const String &string) {
    release();
    if (&string == this) return *this;
    copy(string);
    ++*deleteCount;
    return *this;
}

String &String::operator=(const char *s) {
    if (s == nullptr) {
        mIsNull = true;
        s = "(null)";
    }
    delete[] data;
    newData(s, strlen(s));
    return *this;
}

void String::resize(int newSize) {
    char *newChars = new char[newSize];
    strcpy(newChars, data);
    delete[] data;
    data = nullptr;
    data = newChars;
}

String &String::append(const char *s, size_t size) {
    size_t len = -1;
    String *t = nullptr;
    if (s == this->data) {
        t = new String(s);
        len = t->size();
        s = t->data;
    }
    if (len == -1) len = size;
    if (stringSize + len + 1 > dataSize) {
        dataSize = 2 * (stringSize + len) + 2;
        resize(dataSize);
    }
    for (int i = 0; i < len; ++i)
        data[stringSize + i] = s[i];
    stringSize += len;
    data[stringSize] = '\0';
    delete t;
    return *this;
}

String &String::append(const String &string) {
    append(string.data, string.size());
    return *this;
}

String &String::append(char c) {
    if (stringSize + 2 > dataSize) {
        dataSize = 2 * stringSize + 2;
        resize(dataSize);
    }
    data[stringSize] = c;
    ++stringSize;
    data[stringSize] = '\0';
    return *this;
}

int String::indexOf(char c) {
    return String::indexOf(data, c);
}

int String::indexOf(const char *s) {
    return String::indexOf(data, s);
}

int String::indexOf(const String &string) {
    return indexOf(string.data);
}

int String::indexOf(const char *s, char c) {
    for (int i = 0; s[i] != '\0'; ++i)
        if (s[i] == c) return i;
    return -1;
}

int String::indexOf(const char *haystack, const char *needle) {
    const char *match = strstr(haystack, needle);
    return match == nullptr ? -1 : (int) (match - haystack);
}


SequentialList<String> String::split(const String &separator) const {
    return String::split(*this, separator);
}

SequentialList<String> String::split(const String &str, const String &separator) {
    const char *s = str.getCString();
    int stringSize = str.size();
    SequentialList<String> list;
    SequentialList<int> indexes;
    int length = separator.size();
    const char *found;
    const char *start = s;
    while (start - s <= stringSize && (found = strstr(start, separator.getCString())) != nullptr) {
        indexes.insert((int) (found - s));
        start = found + 1;
    }
    int len = indexes.length();
    int subStart = 0;

    for (int i = 0; i < len; ++i) {
        int index = indexes.get(i);
        int size = index - subStart;
        String r(s + subStart, size);
        list.insert(r);
        subStart = index + length;
    }
    String rest(s + subStart, stringSize - subStart);
    list.insert(rest);
    return list;
}

void String::release() {
    if (deleteCount == nullptr) return;
    if (--*deleteCount == -1) {
        delete[] data;
        data = nullptr;
        delete deleteCount;
    }
}

String::~String() {
    release();
}

String String::toString(int32_t a) {
    String s;
    int x = a;
    while (x != 0) {
        s.insert(0, (char) (x % 10 + 48));
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
    //TODO float toString
    return String();
}

String String::toString(double a) {
    //TODO double toString
    return String();
}

String &String::insert(int index, char c) {
    stringSize += 1;
    if (stringSize + 1 > dataSize) dataSize = 2 * stringSize + 2, resize(dataSize);
    for (int i = stringSize; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = c;
    data[stringSize] = '\0';
    return *this;
}

String &String::insert(int index, const String &string) {
    int len = string.size();
    stringSize += len;
    if (stringSize + 1 > dataSize) dataSize = 2 * stringSize + 2, resize(dataSize);
    for (int i = stringSize; i >= index + len; --i) {
        data[i] = data[i - len];
    }
    for (int i = 0; i < len; ++i) {
        data[index + i] = string.data[i];
    }
    data[stringSize] = '\0';
    return *this;
}

String String::toString(int32_t i, int radix) {
    if (radix < 2 || radix > 36)
        radix = 10;

    /* Use the faster version */
    if (radix == 10) {
        return String::toString(i);
    }

    char buf[33];
    bool negative = (i < 0);
    int charPos = 32;

    if (!negative) {
        i = -i;
    }

    while (i <= -radix) {
        buf[charPos--] = digits[-(i % radix)];
        i = i / radix;
    }
    buf[charPos] = digits[-i];

    if (negative) {
        buf[--charPos] = '-';
    }

    return String(buf + charPos, (33 - charPos));
}

String String::toString(int64_t i, int radix) {
    if (radix < 2 || radix > 36)
        radix = 10;
    if (radix == 10)
        return toString(i);
    char buf[65];
    int charPos = 64;
    bool negative = (i < 0);

    if (!negative) {
        i = -i;
    }

    while (i <= -radix) {
        buf[charPos--] = digits[(int) (-(i % radix))];
        i = i / radix;
    }
    buf[charPos] = digits[(int) (-i)];

    if (negative) {
        buf[--charPos] = '-';
    }

    return String(buf + charPos, (65 - charPos));
}

String String::toString(char c) {
    char s[2];
    s[0] = c, s[1] = '\0';
    return String(s);
}

String::String(int capacity) {
    stringSize = 0, dataSize = capacity + 1;
    data = new char[dataSize];
    data[0] = '\0';
}

void String::clear() {
    stringSize = 0;
    data[0] = '\0';
}

bool String::equals(const String &s) const {
    if (size() != s.size()) return false;
    return String::equal(data, s.data);
}

bool String::equal(const char *s1, const char *s2) {
    for (int i = 0;; ++i) {
        if (s1[i] != s2[i]) return false;
        if (s1[i] == '\0' || s2[i] == '\0') break;
    }
    return true;
}

bool String::isNull() const {
    return mIsNull;
}

String String::toUpperCase(const char *s) {
    String r;
    for (int i = 0; s[i] != '\0'; ++i) {
        r.append((char) toupper(s[i]));
    }
    return r;
}

char String::charAt(int pos) const {
    return data[pos];
}

String String::substring(const char *s, int start, int end) {
    String r;
    for (int i = start; i < end; ++i) {
        r.append(s[i]);
    }
    return r;
}

String String::substring(int start, int end) const {
    return substring(getCString(), start, end);
}

String String::substr(const char *s, int start, int length) {
    return substring(s, start, start + length);
}

String String::substr(int start, int length) const {
    return substr(getCString(), start, length);
}

char *String::toCharArray() const {
    char *r = new char[stringSize];
    for (int i = 0; i < stringSize; ++i) {
        r[i] = data[i];
    }
    return r;
}

String &String::operator+(const String &s) {
    append(s);
    return *this;
}

String String::duplicate() const {
    return String(*this);
}

bool String::operator==(const char *s) const {
    return String::equal(this->getCString(), s);
}

bool String::operator==(const String &s) const {
    return this->equals(s);
}

bool String::operator!=(const char *s) const {
    return !(*this == s);
}

bool String::operator!=(const String &s) const {
    return !(s == *this);
}



