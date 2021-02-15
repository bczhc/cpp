//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_ARRAY_LIST_HPP
#define BCZHC_CPP_ARRAY_LIST_HPP

#include <cstddef>
#include <sys/types.h>

namespace bczhc {
    template<typename T>
    class ArrayList {
    protected:
        size_t len;
        size_t dataSize;

        void resize(size_t newSize) {
            T *newArr = new T[newSize];
            for (size_t i = 0; i < len; ++i)
                newArr[i] = data[i];
            delete[] data;
            data = newArr;
            dataSize = newSize;
        }

    public:
        T *data = nullptr;

        explicit ArrayList(size_t capacity) {
            data = new T[capacity];
            dataSize = capacity;
            len = 0;
        }

        ArrayList() : ArrayList(10) {}

        ArrayList(const ArrayList<T> &a) {
            len = a.len, dataSize = a.dataSize;
            data = new T[dataSize];
            copy(a.data, len);
        }

        ~ArrayList() {
            delete[] data;
        }

        virtual void clear() {
            resize(10);
            len = 0;
        }

        bool isEmpty() const { return len == 0; }

        int length() const { return len; }

        T get(int i) const { return data[i]; }

        virtual void insert(size_t index, T a) {
            if (len == dataSize)
                resize(dataSize * 2 + 2);
            for (size_t i = len; i > index; --i)
                data[i] = data[i - 1];
            data[index] = a;
            ++len;
        }

        virtual void insert(size_t index, const T *a, size_t size) {
            if (index < 0 || index > len) return;
            ssize_t t;
            if (len + size >= dataSize)
                resize(dataSize * 2 + len + size);
            for (size_t i = len - 1 + size; i > index; --i) {
                t = i - size;
                if (t >= 0) data[i] = data[t];
            }
            for (size_t i = index; i < index + size; ++i) {
                data[i] = a[i - index];
            }
            len += size;
        }

        virtual void insert(T a) {
            if (len + 1 >= dataSize)
                resize(2 * (dataSize + 1));
            data[len++] = a;
        }

        virtual void insert(const T *a, size_t size) {
            size_t newSize = dataSize + size;
            if (newSize > dataSize) {
                dataSize = 2 * newSize;
                resize(dataSize);
            }
            for (size_t i = 0; i < size; ++i) {
                data[len + i] = a[i];
            }
            len += size;
        }

        virtual T remove(int index) {
            if (len < dataSize / 4)
                resize(dataSize / 2);
            T removed = data[index];
            --len;
            for (size_t i = index; i < len; ++i)
                data[i] = data[i + 1];
            return removed;
        }

        /**
         * Remove range: [start, end)
         * @param start
         * @param end
         */
        virtual void remove(size_t start, size_t end) {
            if (start < 0 || start >= len || end > len || end < start) return;
            size_t removedSize = end - start;
            size_t t = len - removedSize;
            for (size_t i = start; i < t; ++i) {
                data[i] = data[i + removedSize];
            }
            if (len < dataSize / 4) resize(dataSize / 2);
            len -= removedSize;
        }

        ssize_t indexOf(T t) const {
            for (size_t i = 0; i < len; ++i) {
                if (data[i] == t)
                    return i;
            }
            return -1;
        }

        T &operator[](size_t i) const {
            return data[i];
        }

        void copy(const T *src, size_t size) {
            for (size_t i = 0; i < size; ++i) {
                data[i] = src[i];
            }
        }

        ArrayList<T> &operator=(const ArrayList<T> &a) {
            if (&a == this) return *this;
            delete[] data;
            len = a.len, dataSize = len;
            data = new T[dataSize];
            copy(a.data, len);
            return *this;
        }
    };
}

#endif //BCZHC_CPP_ARRAY_LIST_HPP