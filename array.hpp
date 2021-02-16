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
        struct Properties {
            size_t length = 0;
            T *elements = nullptr;
            int refCount = 0;
        };

        Properties *prop;

        void release() const {
            if (--prop->refCount == -1) {
                delete[] prop->elements;
                delete prop;
            }
        }

    public:

        Array() : Array<T>(0) {}

        Array(size_t length) {
            prop = new Properties;
            if (length != 0) {
                prop->elements = new T[length];
            }
            prop->length = length;
        }

        Array(const Array<T> &a) {
            this->prop = a.prop;
            ++prop->refCount;
        }

        ~Array() {
            release();
        }

        size_t length() const {
            return prop->length;
        }

        Array<T> &operator=(const Array<T> &a) {
            release();
            prop = a.prop;
            ++prop->refCount;
            return *this;
        }

        T &operator[](size_t index) const {
            return prop->elements[index];
        }

        T *getData() const {
            return prop->elements;
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
    };
}

#endif //BCZHC_ARRAY_ARRAY_HPP