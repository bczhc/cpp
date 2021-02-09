//
// Created by zhc on 1/25/21.
//

#ifndef BCZHC_ARRAY_ARRAY_HPP
#define BCZHC_ARRAY_ARRAY_HPP

#include "./string.hpp"
#include "string.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class Array {
    private:
        int len = 0;
        int *deleteCount = nullptr;

        void release() const {
            if (deleteCount == nullptr) return;
            if (--*deleteCount == -1) {
                delete[] elements;
                delete deleteCount;
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

        explicit Array(int size) {
            // Won't allocated when size is zero
            if (size != 0) {
                elements = new T[size];
                deleteCount = new int(0);
            }
            len = size;
        }

        Array(const Array<T> &arr) {
            release();
            this->len = arr.len;
            this->elements = arr.elements;
            this->deleteCount = arr.deleteCount;
            if (deleteCount != nullptr) ++(*deleteCount);
        }

        [[nodiscard]] int length() const {
            return len;
        }

        ~Array() {
            release();
        }

        Array<T> &operator=(const Array<T> &arr) {
            release();
            if (&arr == this) return *this;

            this->len = arr.len;
            this->elements = arr.elements;
            this->deleteCount = arr.deleteCount;
            if (deleteCount != nullptr) ++(*deleteCount);
            return *this;
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