#include "string.hpp"
#include "./utils.hpp"
#include "array_list.hpp"

using namespace bczhc;

void String::copyData(const char *s, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = s[i];
    }
}

void String::copy(const String &s, bool deleteOld) {
    if (deleteOld) delete[] data;
    dataSize = s.stringSize + 1;
    data = new char[dataSize];
    copyData(s.data, s.stringSize);
    stringSize = s.stringSize;
    data[stringSize] = '\0';
}

void String::resize(size_t newSize) {
    char *newArr = new char[newSize];
    for (size_t i = 0; i < stringSize; ++i) {
        newArr[i] = data[i];
    }
    delete[] data;
    data = newArr;
    data[stringSize] = '\0';
    dataSize = newSize;
}

String::String() : String(15) {
}

String::String(const String &string) : String(string.data, string.stringSize) {
}

String::String(const char *s, size_t strSize) : String(s == nullptr ? strlen("(null)") : strSize) {
    if (s == nullptr) s = "(null)";
    copyData(s, strSize);
    stringSize = strSize;
    data[stringSize] = '\0';
}

String::String(const char *s) : String(s, s == nullptr ? 0 : strlen(s)) {
}

String::String(size_t initialCapacity) {
    dataSize = initialCapacity + 1;
    stringSize = 0;
    data = new char[dataSize];
    data[stringSize] = '\0';
}

String::~String() {
    delete[] data;
}

const char *String::getCString() const {
    return data;
}

size_t String::length() const {
    return stringSize;
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

String &String::operator=(const char *s) {
    if (s == nullptr) s = "(null)";
    delete[] data;
    size_t size0 = strlen(s) + 1;
    data = new char[size0];
    // contains '\0';
    copyData(s, size0);
    stringSize = size0 - 1;
    return *this;
}

String &String::operator=(const String &string) {
    if (&string == this) return *this;
    copy(string, true);
    return *this;
}

void String::insert(size_t index, const char *a, size_t size) {
    size_t newSize = stringSize + size;
    if (newSize >= dataSize) resize(2 * newSize + 2);
    stringSize = newSize;
    for (size_t i = stringSize; i >= index + size; --i) {
        data[i] = data[i - size];
    }
    for (size_t i = 0; i < size; ++i) {
        data[index + i] = a[i];
    }
    data[stringSize] = '\0';
}

void String::insert(size_t index, const String &string) {
    insert(index, string.data, string.stringSize);
}

char String::remove(int index) {
    char removed = data[index];
    --stringSize;
    for (size_t i = index; i < stringSize; ++i) {
        data[i] = data[i + 1];
    }
    if (stringSize < dataSize / 4) {
        resize(dataSize / 2);
    }
    data[stringSize] = '\0';
    return removed;
}

/**
 * Remove range: [start, end)
 * @param start
 * @param end
 */
void String::remove(size_t start, size_t end) {
    if (start < 0 || start >= stringSize || end > stringSize || end < start) return;
    size_t removedSize = end - start;
    size_t t = stringSize - removedSize;
    for (size_t i = start; i < t; ++i) {
        data[i] = data[i + removedSize];
    }
    if (stringSize < dataSize / 4) {
        resize(dataSize / 2);
    }
    stringSize -= removedSize;
    data[stringSize] = '\0';
}

String &String::append(const String &string) {
    return append(string.data, string.stringSize);
}

String &String::append(const char *s, size_t size) {
    size_t newSize = stringSize + size;
    if (newSize >= dataSize) {
        resize(newSize * 2 + 2);
    }
    for (size_t i = stringSize; i < stringSize + size; ++i) {
        data[i] = s[i - stringSize];
    }
    stringSize = newSize;
    data[stringSize] = '\0';
    return *this;
}

String &String::append(const char *s) {
    if (s == nullptr) s = "(null)";
    return append(s, strlen(s));
}

String &String::append(char c) {
    return append(&c, 1);
}

String &String::append(int32_t a) {
    return append(String::toString(a));
}

String &String::append(int64_t a) {
    return append(String::toString(a));
}

String &String::appendUnicode(uint32_t codepoint) {
    int size = getUTF8Size(codepoint);
    char s[size];
    unicode2UTF8(s, codepoint);
    this->append(s, size);
    return *this;
}

ssize_t String::indexOf(const char *s) const {
    return indexOf(data, s);
}

ssize_t String::indexOf(const String &string) const {
    return indexOf(data, string.data);
}

ssize_t String::indexOf(const char *s, char c) {
    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (s[i] == c) return i;
    }
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


SP<ArrayList<String>> String::split(const String &separator) const {
    return String::split(*this, separator);
}

SP<ArrayList<String>> String::split(const String &str, const String &separator) {
    const char *s = str.getCString();
    size_t stringSize = str.stringSize;
    auto list = new ArrayList<String>;
    ArrayList<size_t> indexes;
    size_t length = separator.stringSize;
    const char *found;
    const char *start = s;
    while (start - s <= stringSize && (found = strstr(start, separator.getCString())) != nullptr) {
        indexes.add(found - s);
        start = found + 1;
    }
    size_t len = indexes.length();
    size_t subStart = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t index = indexes.get(i);
        size_t size = index - subStart;
        String r(s + subStart, size);
        list->add(r);
        subStart = index + length;
    }
    String rest(s + subStart, stringSize - subStart);
    list->add(rest);
    return list;
}

String String::toString(int32_t a) {
    return Integer::toString(a);
}

String String::toString(int64_t a) {
    return Long::toString(a);
}

String String::toString(float a) {
    // TODO
    return String();
}

String String::toString(double a) {
    // TODO
    return String();
}

void String::insert(size_t index, char c) {
    insert(index, &c, 1);
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

String String::toString(int64_t i, int radix) {
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

String String::toString(uint32_t i, int radix) {
    // TODO
    return String();
}

String String::toString(uint64_t i, int radix) {
    // TODO
    return String();
}

String String::toString(char c) {
    return String(&c, 1);
}

void String::clear() {
    stringSize = 0;
    resize(15);
    data[0] = '\0';
}

void String::toUpperCase(char *s) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        s[i] = (char) toupper(s[i]);
    }
}

String String::toUpperCase(const String &s) {
    size_t len = s.stringSize;
    char b[len + 1];
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        b[i] = (char) toupper(s.data[i]);
    }
    return String(b);
}

String String::toLowerCase(const char *s) {
    return toLowerCase(String(s));
}

String String::toLowerCase(const String &s) {
    size_t len = s.stringSize;
    char b[len + 1];
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        b[i] = (char) tolower(s.data[i]);
    }
    return String(b);
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

SharedPointer<char> String::toCharArray() const {
    char *r = new char[stringSize];
    for (size_t i = 0; i < stringSize; ++i) {
        r[i] = data[i];
    }
    return SP<char>(r);
}

String String::operator+(const String &s) const {
    String r = *this;
    return r += s;
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
    String r = *this;
    return r += c;
}

String &String::operator+=(char c) {
    this->append(c);
    return *this;
}

String &String::operator+=(const char *s) {
    return this->append(s);
}

ssize_t String::lastIndexOf(char c) const {
    for (ssize_t i = length() - 1; i >= 0; --i) {
        if (data[i] == c) return i;
    }
    return -1;
}

char &String::operator[](size_t index) const {
    return data[index];
}

ssize_t String::firstIndexOf(char c) const {
    for (size_t i = 0; i < stringSize; ++i) {
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

String String::toUpperCase() const {
    return String::toUpperCase(*this);
}

String String::toLowerCase() const {
    return String::toLowerCase(*this);
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

