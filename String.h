#include <cstdlib>
#include <cstring>
#include <iostream>

namespace bczhc {
    namespace string {
        class String {
        private:
            char *data = nullptr;
            int len = 0;
            bool willBeCopied = false;
            bool *toModify = &willBeCopied;

            void fromCharsString(const char *s);

        public:
            String(const String &string);

            ~String();

            String(const char *s);

            const char *getCString();

            std::string getCppString();

            int length();

            String &operator=(const char *s);

            String &operator=(const String &string);

            void copy(const String &string);

            void resize(int newLength);

            void append(const char *s);

            void append(char *c);

            int indexOf(char *c);

            int indexOf(const char *s);
        };
    }// namespace string
}// namespace bczhc
