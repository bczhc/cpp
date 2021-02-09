//
// Created by bczhc on 2/8/21.
//

#include "./exception.hpp"
#include "string.hpp"

bczhc::NumberFormatException bczhc::NumberFormatException::forInputString(const String &s) {
    return NumberFormatException(String("For input string: \"") + s + "\"");
}

bczhc::NumberFormatException::NumberFormatException(const char *msg) : Exception(msg) {}

bczhc::NumberFormatException::NumberFormatException(const String &msg) : Exception(msg) {}

bczhc::NumberFormatException
bczhc::NumberFormatException::forCharSequence(const String &s, int32_t beginIndex, int32_t endIndex,
                                              int32_t errorIndex) {
    String msg = String("Error at index ") + String::toString(errorIndex - beginIndex) + " in: \"";
    msg += s.substring(beginIndex, endIndex) + '\"';
    return NumberFormatException(msg);
}

bczhc::NullPointerException::NullPointerException(const char *msg) : Exception(msg) {}

bczhc::NullPointerException::NullPointerException(const String &msg) : Exception(msg) {}

bczhc::NullPointerException::NullPointerException() = default;

bczhc::IndexOutOfBoundsException::IndexOutOfBoundsException(const char *msg) : Exception(msg) {}

bczhc::IndexOutOfBoundsException::IndexOutOfBoundsException(const String &msg) : Exception(msg) {}

bczhc::IndexOutOfBoundsException::IndexOutOfBoundsException() = default;

bczhc::SqliteException::SqliteException(const char *msg, int returnCode) : Exception(msg), returnCode(returnCode) {}
