#ifndef BCZHC_STRING_HPP
#define BCZHC_STRING_HPP

#include "./third_party/practice/LinearList.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "utf8.h"
#include <cstdint>
#include <sys/types.h>

using namespace bczhc;

namespace bczhc {
    class String {
    private:
        char *data = nullptr;
        size_t stringSize = 0, dataSize = 0;
        bool mIsNull = false;

        void newData(const char *s, size_t strSize, size_t capacity);

        void newData(const char *s, size_t size);

        void fromCharsString(const char *s, size_t size);

        void fromCharsString(const char *s, size_t strSize, ssize_t capacity);

        void resize(size_t newStringLength);

        void release();


    public:
        int *deleteCount = nullptr;

        String();

        String(const String &string);

        String(const char *s, size_t size);

        String(const char *s); // NOLINT(google-explicit-constructor)

        ~String();

        [[nodiscard]] const char *getCString() const;

        [[nodiscard]] size_t utf8Length() const;

        [[nodiscard]] size_t length() const;

        String &operator=(const String &string);

        String &operator=(const char *s);

        void copy(const String &string);

        String &append(const String &string);

        String &append(char c);

        String &append(const char *s, size_t size);

        String &append(int32_t a);

        String &append(int64_t a);

        [[nodiscard]] ssize_t indexOf(char c) const;

        ssize_t indexOf(const char *s) const;

        [[nodiscard]] ssize_t indexOf(const String &string) const;

        static ssize_t indexOf(const char *s, char c);

        static ssize_t indexOf(const char *haystack, const char *needle);

        [[nodiscard]] SequentialList<String> split(const String &separator) const;

        static SequentialList<String> split(const String &str, const String &separator);

        static String toString(int32_t a);

        static String toString(int64_t a);

        static String toString(float a);

        static String toString(double a);

        String &insert(size_t index, char c);

        String &insert(size_t index, const String &string);

        static String toString(int32_t i, int radix);

        static String toString(int64_t i, int radix);

        static String toString(char c);

        explicit String(size_t initialCapacity);

        void clear();

        static String toUpperCase(const char *s);

        [[nodiscard]] bool equals(const String &s) const;

        static bool equal(const char *s1, const char *s2);

        [[nodiscard]] bool isNull() const;

        [[nodiscard]] char charAt(size_t pos) const;

        static String substr(const char *s, size_t start, size_t length);

        [[nodiscard]] String substr(size_t start, size_t length) const;

        static String substring(const char *s, size_t start, size_t end);

        [[nodiscard]] String substring(size_t start, size_t end) const;

        [[nodiscard]] char *toCharArray() const;

        String operator+(const String &s) const;

        String operator+(char c) const;

        [[nodiscard]] String duplicate() const;

        bool operator==(const char *s) const;

        bool operator!=(const char *s) const;

        bool operator==(const String &s) const;

        bool operator!=(const String &s) const;

        String &operator+=(const String &s);

        String &operator+=(char c);

        [[nodiscard]] ssize_t lastIndexOf(char c) const;

        [[nodiscard]] ssize_t firstIndexOf(char c) const;

        char &operator[](size_t index);

        friend String operator+(const char *s, const String &o) {
            return String(s) + o;
        }

        friend bool operator==(const char *s, const String &o) {
            return o == s;
        }

        friend bool operator!=(const char *s, const String &o) {
            return o != s;
        }

        String operator+(int32_t a) const;

        String operator+(int64_t a) const;

        String &operator+=(int32_t a);

        String &operator+=(int64_t a);
    };
}
#endif//BCZHC_STRING_HPP