#ifndef BCZHC_STRING_HPP
#define BCZHC_STRING_HPP

#include "./array_list.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <sys/types.h>
#include "./shared_pointer.hpp"
#include "./utf8.hpp"

using namespace bczhc;

namespace bczhc {
    class String : public ArrayList<char> {
    private:
        struct Properties {
            bool isNull = false;
        };

        Properties *mInfo = nullptr;

        using Super = ArrayList<char>;

        void checkSpaceAndResize(size_t newSize);

        void mCopy(const String &s);

        void copyData(const char *s, size_t size);

        void checkSizeForTerminateCharAndResize();

    public:
        String();

        String(const String &string);

        String(const char *s, size_t size);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

        String(const char *s);

#pragma clang diagnostic pop

        explicit String(size_t initialCapacity);

        ~String();

        [[nodiscard]] const char *getCString() const;

        [[nodiscard]] size_t utf8Length() const;

        String &operator=(const String &string);

        void mRelease() const;

        String &operator=(const char *s);

        void insert(size_t index, const char *a, size_t size) override;

        void insert(char a) override;

        void insert(const char *a, size_t size) override;

        char remove(int index) override;

        void remove(size_t start, size_t end) override;

        String &append(const String &string);

        String &append(const char *s);

        String &append(char c);

        String &append(const char *s, size_t size);

        String &append(int32_t a);

        String &append(int64_t a);

        String &appendUnicode(uint32_t codepoint);

        ssize_t indexOf(const char *s) const;

        [[nodiscard]] ssize_t indexOf(const String &string) const;

        static ssize_t indexOf(const char *s, char c);

        static ssize_t indexOf(const char *haystack, const char *needle);

        [[nodiscard]] ArrayList<String> split(const String &separator) const;

        static ArrayList<String> split(const String &str, const String &separator);

        static String toString(int32_t a);

        static String toString(int64_t a);

        static String toString(float a);

        static String toString(double a);

        void insert(size_t index, char c) override;

        void insert(size_t index, const String &string);

        static String toString(int32_t i, int radix);

        static String toString(int64_t i, int radix);

        static String toString(uint32_t i, int radix);

        static String toString(uint64_t i, int radix);

        static String toString(char c);

        void clear() override;

        static String toUpperCase(const char *s);

        static String toUpperCase(const String &s);

        String toUpperCase() const;

        static String toLowerCase(const char *s);

        static String toLowerCase(const String &s);

        String toLowerCase() const;

        [[nodiscard]] bool equals(const String &s) const;

        static bool equal(const char *s1, const char *s2);

        [[nodiscard]] bool isNull() const;

        [[nodiscard]] char charAt(size_t pos) const;

        static String substr(const char *s, size_t start, size_t length);

        [[nodiscard]] String substr(size_t start, size_t length) const;

        static String substring(const char *s, size_t start, size_t end);

        [[nodiscard]] String substring(size_t start, size_t end) const;

        [[nodiscard]] SharedPointer<char> toCharArray() const;

        String operator+(const String &s) const;

        String operator+(char c) const;

        bool operator==(const char *s) const;

        bool operator!=(const char *s) const;

        bool operator==(const String &s) const;

        bool operator!=(const String &s) const;

        String &operator+=(const String &s);

        String &operator+=(char c);

        String &operator+=(const char *s);

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

        String duplicate() const;
    };
}
#endif//BCZHC_STRING_HPP