#include <cstdlib>
#include <cstring>
#include <iostream>

namespace bczhc {
    namespace string {
        class String {
        private:
            char *data = nullptr;
            int stringSize = 0, dataSize = 0;
            bool willBeCopied = false;
            bool *toModify = &willBeCopied;

            void fromCharsString(const char *s);

            void resize(int newLength);

        public:
            String();

            String(const String &string);

            ~String();

            String(const char *s);

            const char *getCString();

            std::string getCppString();

            int length();

            int size();

            String &operator=(const char *s);

            String &operator=(const String &string);

            void copy(const String &string);

            void append(const char *s);

            void append(const String &string);

            void append(char c);

            int indexOf(char c);

            int indexOf(const char *s);

            int indexOf(const String &string);
        };
    }// namespace string
}// namespace bczhc
