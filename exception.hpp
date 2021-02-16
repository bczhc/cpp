//
// Created by bczhc on 2/8/21.
//

#ifndef BCZHC_EXCEPTION_HPP
#define BCZHC_EXCEPTION_HPP

#include <exception>
#include "./string.hpp"

using namespace bczhc;

namespace bczhc {
    class Exception : public std::exception {
    private:
        String msg{};
    public:
        Exception() = default;

        explicit Exception(const char *msg);

        explicit Exception(const String &msg);

        ~Exception() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override;

        Exception &operator=(const Exception &e);

        Exception(const Exception &e);
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

        SqliteException(const String &msg, int returnCode);

        explicit SqliteException(int returnCode) : returnCode(returnCode) {}
    };

    class IOException : public Exception {
    public:
        explicit IOException(const char *msg);

        explicit IOException(const String &msg);

        IOException();
    };

    class NoSuchElementException : public Exception {
    public:
        NoSuchElementException();
    };

    class PCREException: public Exception {
    public:
        explicit PCREException(const char *msg);

        explicit PCREException(const String &msg);
    };
}

#endif //BCZHC_EXCEPTION_HPP
