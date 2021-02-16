#include "string.hpp"
#include "./utils.hpp"

using namespace bczhc;

void String::checkSpaceAndResize(size_t newSize) {
    if (newSize > prop->dataSize) {
        resize(newSize * 2);
    }
}

void String::mCopy(const String &s) {
    mInfo = s.mInfo;
}

void String::copyData(const char *s, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        prop->data[i] = s[i];
    }
}

void String::checkSizeForTerminateCharAndResize() {
    if (prop->dataSize < prop->length + 1) {
        resize(prop->dataSize + 1);
    }
}

String::String() : String(15) {}

String::String(const String &string) : Super(string) {
    mCopy(string);
}

String::String(const char *s, size_t size) : Super(s == nullptr ? (strlen("(null)") + 1) : (size + 1)) {
    copyData(s == nullptr ? "(null)" : s, prop->dataSize - 1);
    prop->length = prop->dataSize - 1;
    prop->data[prop->length] = '\0';
    mInfo = new Properties;
}

String::String(const char *s) : String(s, s == nullptr ? 0 : strlen(s)) {}

String::String(size_t initialCapacity) : Super(initialCapacity < 1 ? 1 : initialCapacity) {
    prop->data[0] = '\0';
    mInfo = new Properties;
}

String::~String() {
    mRelease();
}

const char *String::getCString() const {
    return this->prop->data;
}

size_t String::utf8Length() const {
    size_t c = 0;
    size_t i = 0;
    while (prop->data[i] != '\0') {
        i += getUTF8BytesLength(prop->data[i]);
        ++c;
    }
    return c;
}

String &String::operator=(const String &string) {
    if (&string == this) return *this;
    mRelease();
    // contains ++refCount
    Super::operator=(string);
    mCopy(string);
    return *this;
}

void String::mRelease() const { if (prop->refCount == 0) delete mInfo; }

String &String::operator=(const char *s) {
    Super::release();
    delete mInfo;
    prop = new Super::Properties;
    mInfo = new Properties;
    if (s == nullptr) {
        s = "(null)";
        mInfo->isNull = true;
    }
    prop->length = strlen(s);
    prop->dataSize = prop->length + 1;
    prop->data = new char[prop->dataSize];
    // contains '\0';
    copyData(s, prop->dataSize);
    return *this;
}

void String::insert(size_t index, const char *a, size_t size) {
    checkSpaceAndResize(prop->length + size + 1);
    Super::insert(index, a, size);
    prop->data[prop->length] = '\0';
}

void String::insert(char a) {
    checkSpaceAndResize(prop->length + 2);
    Super::insert(a);
    prop->data[prop->length] = '\0';
}

void String::insert(const char *a, size_t size) {
    checkSpaceAndResize(prop->length + size + 1);
    Super::insert(a, size);
    prop->data[prop->length] = '\0';
}

char String::remove(int index) {
    auto removed = Super::remove(index);
    checkSizeForTerminateCharAndResize();
    prop->data[prop->length] = '\0';
    return removed;
}

void String::remove(size_t start, size_t end) {
    Super::remove(start, end);
    checkSizeForTerminateCharAndResize();
    prop->data[prop->length] = '\0';
}

String &String::append(const String &string) {
    append(string.getCString(), string.length());
    return *this;
}

String &String::append(char c) {
    insert(c);
    return *this;
}

String &String::append(const char *s, size_t size) {
    insert(s, size);
    return *this;
}

String &String::append(int32_t a) {
    append(String::toString(a));
    return *this;
}

String &String::append(int64_t a) {
    append(String::toString(a));
    return *this;
}

String &String::appendUnicode(uint32_t codepoint) {
    int size = getUTF8Size(codepoint);
    char s[size];
    unicode2UTF8(s, codepoint);
    this->append(s, size);
    return *this;
}

ssize_t String::indexOf(const char *s) const {
    return String::indexOf(getCString(), s);
}

ssize_t String::indexOf(const String &string) const {
    return indexOf(getCString(), string.getCString());
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
    checkSpaceAndResize(prop->length + 2);
    Super::insert(index, c);
    prop->data[prop->length] = '\0';
}

void String::insert(size_t index, const String &string) {
    insert(index, string.getCString(), string.length());
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
    Super::clear();
    checkSizeForTerminateCharAndResize();
    prop->data[prop->length] = '\0';
}

String String::toUpperCase(const char *s) {
    return toUpperCase(String(s));
}

String String::toUpperCase(const String &s) {
    size_t len = s.length();
    char b[len + 1];
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        b[i] = (char) toupper(s.prop->data[i]);
    }
    return String(b);
}

String String::toLowerCase(const char *s) {
    return toLowerCase(String(s));
}

String String::toLowerCase(const String &s) {
    size_t len = s.length();
    char b[len + 1];
    String r(len);
    for (size_t i = 0; i < len; ++i) {
        b[i] = (char) tolower(s.prop->data[i]);
    }
    return String(b);
}

char String::charAt(size_t pos) const {
    return prop->data[pos];
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
    char *r = new char[prop->length];
    for (size_t i = 0; i < prop->length; ++i) {
        r[i] = prop->data[i];
    }
    return SP<char>(r);
}

String String::operator+(const String &s) const {
    return this->duplicate() += s;
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
        if (prop->data[i] == c) return i;
    }
    return -1;
}

char &String::operator[](size_t index) {
    return prop->data[index];
}

ssize_t String::firstIndexOf(char c) const {
    for (size_t i = 0; i < prop->length; ++i) {
        if (prop->data[i] == c) return i;
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
    return String::equal(prop->data, s.prop->data);
}

bool String::equal(const char *s1, const char *s2) {
    for (size_t i = 0;; ++i) {
        if (s1[i] != s2[i]) return false;
        if (s1[i] == '\0' || s2[i] == '\0') break;
    }
    return true;
}

bool String::isNull() const {
    return mInfo->isNull;
}

String String::duplicate() const {
    return String(getCString(), length());
}
