//
// Created by zhc on 1/25/21.
//

#ifndef BCZHC_ARRAY_ARRAY_HPP
#define BCZHC_ARRAY_ARRAY_HPP

#include "./string.hpp"
#include <cstdarg>

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class Array {
    private:
        size_t len = 0;
        T *elements = nullptr;

        void copyElem(const T *a, size_t length) const {
            for (size_t i = 0; i < length; ++i) {
                elements[i] = a[i];
            }
        }

        void copy(const Array<T> &a, bool deleteOld = false) {
            if (deleteOld) delete[] elements;
            elements = new T[a.len];
            this->len = a.len;
            copyElem(a.elements, a.len);
        }

    public:

        Array() : Array<T>(0) {}

        Array(size_t length) {
            if (length != 0) {
                elements = new T[length];
            }
            len = length;
        }

        Array(const Array<T> &a) {
            copy(a);
        }

        ~Array() {
            delete[] elements;
        }

        size_t length() const {
            return len;
        }

        Array<T> &operator=(const Array<T> &a) {
            if (&a == this) return *this;
            copy(a, true);
            return *this;
        }

        T &operator[](size_t index) const {
            return elements[index];
        }

        T *data() const {
            return elements;
        }

        bool operator==(const Array<T> &a) const {
            if (this->length() != a.length()) return false;
            for (int i = 0; i < a.length(); ++i) {
                if ((*this)[i] != a[i]) return false;
            }
            return true;
        }

        bool operator!=(const Array<T> &a) const {
            return !((*this) == a);
        }

        static Array<T> from(T *arr, int size) {
            Array<T> r(size);
            for (int i = 0; i < size; ++i) {
                r[i] = arr[i];
            }
            return r;
        }

        [[nodiscard]] String toString() const {
            return Array<T>::toString(this->elements, this->len, 10);
        }

        [[nodiscard]] String toString(int radix) const {
            return Array<T>::toString(this->elements, this->len, radix);
        }

        [[nodiscard]] String toString(int radix, const char *separator) const {
            return Array<T>::toString(this->elements, this->len, radix, separator);
        }

        [[nodiscard]] static String toString(T *arr, size_t size, int radix) {
            return toString(arr, size, radix, ", ");
        }

        [[nodiscard]] static String toString(T *arr, size_t size, int radix, const char *separator) {
            String msg = "[";
            for (int i = 0; i < size; ++i) {
                msg += String::toString(arr[i], radix);
                if (i != size - 1) {
                    msg += separator;
                }
            }
            msg += ']';
            return msg;
        }

        template<size_t len_>
        struct Elements {
            T elem[len_];
        };

        template<size_t len_>
        static Array<T> makeArray(const Elements<len_> &a) {
            Array<T> r(len_);
            for (int i = 0; i < len_; ++i) {
                r[i] = a.elem[i];
            }
            return r;
        }

        template<size_t len_>
        Array<T> &set(const Elements<len_> &a) {
            delete[] elements;
            elements = new T[len_];
            copyElem(a.elem, len_);
            return *this;
        }

    };
}

#endif //BCZHC_ARRAY_ARRAY_HPP