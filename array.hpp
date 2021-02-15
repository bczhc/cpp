//
// Created by zhc on 1/25/21.
//

#ifndef BCZHC_ARRAY_ARRAY_HPP
#define BCZHC_ARRAY_ARRAY_HPP

#include "./string.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class Array {
    private:
        int len = 0;
        int *refCount = nullptr;

        void release() const {
            if (--*refCount == -1) {
                delete[] elements;
                delete refCount;
            }
        }

    public:
        T *elements = nullptr;

        static Array<T> from(T *arr, int size) {
            Array<T> r(size);
            for (int i = 0; i < size; ++i) {
                r[i] = arr[i];
            }
            return r;
        }

        Array() : Array(0) {}

        explicit Array(int size) {
            // Won't allocated when size is zero
            if (size != 0) {
                elements = new T[size];
            }
            refCount = new int(0);
            len = size;
        }

        Array(const Array<T> &arr) {
            copy(arr);
            ++(*refCount);
        }

        [[nodiscard]] int length() const {
            return len;
        }

        ~Array() {
            release();
        }

        Array<T> &operator=(const Array<T> &arr) {
            if (&arr == this) return *this;
            release();
            copy(arr);
            ++(*refCount);
            return *this;
        }

        void copy(const Array <T> &arr) {
            len = arr.len;
            elements = arr.elements;
            refCount = arr.refCount;
        }

        T &operator[](int i) const {
            return this->elements[i];
        }

        bool operator==(const Array<T> &a) const {
            if (a.length() != this->len) return false;
            for (int i = 0; i < len; ++i) {
                if (elements[i] != a[i]) return false;
            }
            return true;
        }

        bool operator!=(const Array<T> &a) const {
            return !(a == *this);
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
    };
}

#endif //BCZHC_ARRAY_ARRAY_HPP