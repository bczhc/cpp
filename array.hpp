//
// Created by zhc on 1/25/21.
//

#ifndef CPP_ARRAY_HPP
#define CPP_ARRAY_HPP

namespace bczhc::array {
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
        T *elements;

        static Array<T> from(T *arr, int size) {
            Array<T> r(size);
            for (int i = 0; i < size; ++i) {
                r[i] = arr[i];
            }
            return r;
        }

        explicit Array(int size) {
            if (size != 0) elements = new T[size];
            len = size;
            deleteCount = new int(0);
        }

        Array(const Array<T> &arr) {
            release();
            this->len = arr.len;
            this->elements = arr.elements;
            this->deleteCount = arr.deleteCount;
            ++(*deleteCount);
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
            ++(*deleteCount);
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
    };
}

#endif //CPP_ARRAY_HPP