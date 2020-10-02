#ifndef BCZHC_STRING_H
#define BCZHC_STRING_H

#include "./third_party/practice/LinearList.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace bczhc::linearlist;

namespace bczhc {
    namespace string {
        class String {
        private:
            char *data = nullptr;
            int stringSize = 0, dataSize = 0;
            bool willBeCopied = false;
            bool *toModify = &willBeCopied;

            void fromCharsString(const char *s, size_t stringSize);

            void resize(int newStringLength);

        public:
            String();

            String(const String &string);

            String(const std::string &str);

            String(const char *s, int size);

            ~String();

            String(const char *s);

            const char *getCString() const;

            std::string getCppString();

            int length();

            int size() const;

            String &operator=(const char *s);

            String &operator=(const String &string);

            String &operator=(const std::string &str);

            void copy(const String &string);

            String &append(const char *s);

            String &append(const String &string);

            String &append(char c);

            String &append(const std::string &s);

            int indexOf(char c);

            int indexOf(const char *s);

            int indexOf(const String &string);

            SequentialList<String> split(const String &separator);

            SequentialList<String> split(const char *separator);
        };
    }// namespace string
}// namespace bczhc

#endif //BCZHC_STRING_H