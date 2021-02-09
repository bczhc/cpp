//
// Created by bczhc on 2/8/21.
//

#ifndef BCZHC_EXCEPTION_HPP
#define BCZHC_EXCEPTION_HPP

#include <exception>
#include "string.hpp"

#ifndef BCZHC_STRING_HPP

#include "./string.hpp"

#endif

using namespace bczhc;

namespace bczhc {
    class Exception : public std::exception {
    private:
        String msg{};
    public:
        Exception() = default;

        explicit Exception(const char *msg) {
            this->msg = msg;
        }

        explicit Exception(const String &msg) {
            this->msg = msg;
        }

        ~Exception() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override {
            return msg.getCString();
        }

        Exception(const Exception &e) {
            this->msg = e.msg;
        }
    };

    class NumberFormatException : public Exception {
    public:
        explicit NumberFormatException(const char *msg);

        explicit NumberFormatException(const String &msg);

        static NumberFormatException forInputString(const String &s);

        static NumberFormatException
        forCharSequence(const String &s, int32_t beginIndex, int32_t endIndex, int32_t errorIndex);
    };

    class NullPointerException : public Exception {
    public:
        explicit NullPointerException(const char *msg);

        explicit NullPointerException(const String &msg);

        NullPointerException();
    };

    class IndexOutOfBoundsException : public Exception {
    public:
        explicit IndexOutOfBoundsException(const char *msg);

        explicit IndexOutOfBoundsException(const String &msg);

        IndexOutOfBoundsException();
    };

    class SqliteException : public Exception {
    public:
        int returnCode;
        SqliteException(const char *msg, int returnCode);
        explicit SqliteException(int returnCode) : returnCode(returnCode) {}
    };
}

#endif //BCZHC_EXCEPTION_HPP
