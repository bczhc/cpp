#include "string.hpp"
#include <cstddef>
#include <cstring>
#include "utils.hpp"

using namespace bczhc;

String::String() {
    fromCharsString(nullptr, 0);
}

String::String(const String &string) {
    copy(string);
    ++*refCount;
}

void String::copy(const String &string) {
    refCount = string.refCount;
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

size_t String::utf8Length() const {
    size_t c = 0;
    size_t i = 0;
    while (data[i] != '\0') {
        i += getUTF8BytesLength(data[i]);
        ++c;
    }
    return c;
}

size_t String::length() const {
    return stringSize;
}

void String::newData(const char *s, size_t strSize, size_t capacity) {
    if (capacity < strSize + 1) throw Exception("illegal capacity");
    stringSize = strSize, dataSize = capacity;
    data = new char[dataSize];
    for (size_t i = 0; i < strSize; ++i) data[i] = s[i];
    data[strSize] = '\0';
}

void String::newData(const char *s, size_t size) {
    newData(s, size, size + 1);
}

void String::fromCharsString(const char *s, size_t strSize) {
    fromCharsString(s, strSize, strSize + 1);
}

void String::fromCharsString(const char *s, size_t strSize, ssize_t capacity) {
    refCount = new int(0);
    newData(s, strSize, capacity);
}


String &String::operator=(const String &string) {
    if (&string == this) return *this;
    release();
    copy(string);
    ++*refCount;
    return *this;
}

String &String::operator=(const char *s) {
    release();
    if (s == nullptr) {
        mIsNull = true;
        s = "(null)";
    }
    fromCharsString(s, strlen(s));
    return *this;
}

void String::resize(size_t newSize) {
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
        len = t->length();
        s = t->data;
    }
    if (len == -1) len = size;
    if (stringSize + len + 1 > dataSize) {
        dataSize = 2 * (stringSize + len) + 2;
        resize(dataSize);
    }
    for (size_t i = 0; i < len; ++i)
        data[stringSize + i] = s[i];
    stringSize += len;
    data[stringSize] = '\0';
    delete t;
    return *this;
}

String &String::append(const String &string) {
    append(string.data, string.length());
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

ssize_t String::indexOf(char c) const {
    return String::indexOf(data, c);
}

ssize_t String::indexOf(const char *s) const {
    return String::indexOf(data, s);
}

ssize_t String::indexOf(const String &string) const {
    return indexOf(string.data);
}

ssize_t String::indexOf(const char *s, char c) {
    for (size_t i = 0; s[i] != '\0'; ++i)
        if (s[i] == c) return i;
    return -1;
}

ssize_t String::indexOf(const char *haystack, const char *needle) {
    if (haystack == nullptr || needle == nullptr) return -1;
    for (int i = 0; haystack[i] != '\0'; ++i) {
        bool match = true;
        for (int j = 0; needle[j] != '\0'; ++j) {
            if (haystack[i + j] != needle[j]) match = false;
        }
        if (match) return i;
    }
    return -1;
}


ArrayList<String> String::split(const String &separator) const {
    return String::split(*this, separator);
}

ArrayList<String> String::split(const String &str, const String &separator) {
    const char *s = str.getCString();
    size_t stringSize = str.length();
    ArrayList<String> list;
    ArrayList<size_t> indexes;
    size_t length = separator.length();
    const char *found;
    const char *start = s;
    while (start - s <= stringSize && (found = strstr(start, separator.getCString())) != nullptr) {
        indexes.insert(found - s);
        start = found + 1;
    }
    size_t len = indexes.length();
    size_t subStart = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t index = indexes.get(i);
        size_t size = index - subStart;
        String r(s + subStart, size);
        list.insert(r);
        subStart = index + length;
    }
    String rest(s + subStart, stringSize - subStart);
    list.insert(rest);
    return list;
}

void String::release() {
    if (--*refCount == -1) {
        delete[] data;
        data = nullptr;
        delete refCount;
    }
}

String::~String() {
    release();
}

String String::toString(int32_t a) {
    return Integer::toString(a);
}

String String::toString(int64_t a) {
    return Long::toString(a);
}

String String::toString(float a) {
    //TODO float toString
    char s[50] = {0};
    sprintf(s, "%.7f", a);
    return String(s);
}

String String::toString(double a) {
    //TODO double toString
    char s[50] = {0};
    sprintf(s, "%.23lf", a);
    return String(s);
}

String &String::insert(size_t index, char c) {
    stringSize += 1;
    if (stringSize + 1 > dataSize) dataSize = 2 * stringSize + 2, resize(dataSize);
    for (size_t i = stringSize; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = c;
    data[stringSize] = '\0';
    return *this;
}

String &String::insert(size_t index, const String &string) {
    size_t len = string.length();
    stringSize += len;
    if (stringSize + 1 > dataSize) dataSize = 2 * stringSize + 2, resize(dataSize);
    for (size_t i = stringSize; i >= index + len; --i) {
        data[i] = data[i - len];
    }
    for (size_t i = 0; i < len; ++i) {
        data[index + i] = string.data[i];
    }
    data[stringSize] = '\0';
    return *this;
}

String String::toString(int32_t i, int32_t radix) {
    if (radix < 2 || radix > 36)
        radix = 10;

    /* Use the faster version */
    if (radix == 10) {
        return String::toString(i);
    }

    char buf[33];
    bool negative = (i < 0);
    int32_t charPos = 32;

    if (!negative) {
        i = -i;
    }

    while (i <= -radix) {
        buf[charPos--] = Integer::digits[-(i % radix)];
        i = i / radix;
    }
    buf[charPos] = Integer::digits[-i];

    if (negative) {
        buf[--charPos] = '-';
    }

    return String(buf + charPos, (33 - charPos));
}

String String::toString(int64_t i, int32_t radix) {
    if (radix < 2 || radix > 36)
        radix = 10;
    if (radix == 10)
        return toString(i);
    char buf[65];
    int32_t charPos = 64;
    bool negative = (i < 0);

    if (!negative) {
        i = -i;
    }

    while (i <= -radix) {
        buf[charPos--] = Integer::digits[(int32_t) (-(i % radix))];
        i = i / radix;
    }
    buf[charPos] = Integer::digits[(int32_t) (-i)];

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

String::String(size_t initialCapacity) {
    fromCharsString(nullptr, 0, initialCapacity);
}

void String::clear() {
    stringSize = 0;
    data[0] = '\0';
}

bool String::equals(const String &s) const {
    if (length() != s.length()) return false;
    return String::equal(data, s.data);
}

bool String::equal(const char *s1, const char *s2) {
    for (size_t i = 0;; ++i) {
        if (s1[i] != s2[i]) return false;
        if (s1[i] == '\0' || s2[i] == '\0') break;
    }
    return true;
}

bool String::isNull() const {
    return mIsNull;
}

String String::toUpperCase(const char *s) {
    return toUpperCase(String(s));
}

String String::toUpperCase(const String &s) {
    size_t len = s.length();
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        r += (char) toupper(s.data[i]);
    }
    return r;
}

String String::toLowerCase(const char *s) {
    return toLowerCase(String(s));
}

String String::toLowerCase(const String &s) {
    size_t len = s.length();
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        r += (char) tolower(s.data[i]);
    }
    return r;
}

char String::charAt(size_t pos) const {
    return data[pos];
}

String String::substring(const char *s, size_t start, size_t end) {
    String r;
    for (size_t i = start; i < end; ++i) {
        r.append(s[i]);
    }
    return r;
}

String String::substring(size_t start, size_t end) const {
    return substring(getCString(), start, end);
}

String String::substr(const char *s, size_t start, size_t length) {
    return substring(s, start, start + length);
}

String String::substr(size_t start, size_t length) const {
    return substr(getCString(), start, length);
}

char *String::toCharArray() const {
    char *r = new char[stringSize];
    for (size_t i = 0; i < stringSize; ++i) {
        r[i] = data[i];
    }
    return r;
}

String String::operator+(const String &s) const {
    return this->duplicate() += s;
}

String String::duplicate() const {
    return String(this->data);
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

String &String::operator+=(const String &s) {
    this->append(s);
    return *this;
}

String String::operator+(char c) const {
    return this->duplicate() += c;
}

String &String::operator+=(char c) {
    this->append(c);
    return *this;
}

ssize_t String::lastIndexOf(char c) const {
    for (ssize_t i = length() - 1; i >= 0; --i) {
        if (data[i] == c) return i;
    }
    return -1;
}

char &String::operator[](size_t index) {
    return data[index];
}

ssize_t String::firstIndexOf(char c) const {
    for (size_t i = 0; i < length(); ++i) {
        if (data[i] == c) return i;
    }
    return -1;
}

String String::operator+(int32_t a) const {
    return (*this) + String::toString(a);
}

String String::operator+(int64_t a) const {
    return (*this) + String::toString(a);
}

String &String::operator+=(int32_t a) {
    return this->append(a);
}

String &String::operator+=(int64_t a) {
    return this->append(a);
}

String &String::append(int64_t a) {
    return this->append(String::toString(a));
}

String &String::append(int32_t a) {
    return this->append(String::toString(a));
}

String &String::appendUnicode(uint32_t codepoint) {
    int size = getUTF8Size(codepoint);
    char s[size];
    unicode2UTF8(s, codepoint);
    this->append(s, size);
    return *this;
}

String String::toUpperCase() const {
    return String::toUpperCase(*this);
}

String String::toLowerCase() const {
    return String::toLowerCase(*this);
}
