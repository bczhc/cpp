#ifndef BCZHC_STRING_H
#define BCZHC_STRING_H

#include "./third_party/practice/LinearList.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace bczhc::linearlist;

namespace bczhc {
    namespace string {
        static char digits[] = {
                '0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'a', 'b',
                'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'l', 'm', 'n',
                'o', 'p', 'q', 'r', 's', 't',
                'u', 'v', 'w', 'x', 'y', 'z'};

        class String {
        private:
            char *data = nullptr;
            size_t stringSize = 0, dataSize = 0;

            void fromCharsString(const char *s, size_t stringSize);

            void resize(int newStringLength);

            String &append(const char *s, size_t size);

        public:
            bool copyData = true;

            String();

            String(const String &string);

            String(const std::string &str);

            String(const char *s, size_t size);

            String(const char *s);

            ~String();

            const char *getCString() const;

            std::string getCppString();

            int length();

            size_t size() const;

            String &operator=(const String &string);

            void copy(const String &string);

            String &append(const String &string);

            String &append(char c);

            int indexOf(char c);

            int indexOf(const char *s);

            int indexOf(const String &string);

            static int indexOf(const char *s, char c);

            static int indexOf(const char *haystack, const char *needle);


            SequentialList<String> split(const String &separator) const;

            static SequentialList<String> split(const String &str, const String &separator);

            static String toString(int32_t a);

            static String toString(int64_t a);

            static String toString(float a);

            static String toString(double a);

            String &insert(int index, char c);

            String &insert(int index, const String &string);

            static String toString(int32_t i, int radix);

            static String toString(int64_t i, int radix);

            static String toString(char c);

            String(int capacity);

            void clear();
        };
    }// namespace string
}// namespace bczhc

#endif//BCZHC_STRING_H
