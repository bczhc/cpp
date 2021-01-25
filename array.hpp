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

        explicit Array(int size) {
            elements = new T[size];
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

        int length() const {
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
    };
}

#endif //CPP_ARRAY_HPP