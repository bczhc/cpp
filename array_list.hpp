//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_ARRAY_LIST_HPP
#define BCZHC_CPP_ARRAY_LIST_HPP

#include <cstddef>
#include <sys/types.h>
#include "collection.hpp"
#include "list.hpp"
#include "exception.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class ArrayList : public List<T> {
    private:
        void copyElem(const T *a, size_t length) {
            for (int i = 0; i < length; ++i) {
                data[i] = a[i];
            }
        }

        void copy(const ArrayList<T> &a, bool deleteOld = false) {
            if (deleteOld) delete[] data;
            this->len = a.len;
            this->dataSize = this->len;
            data = new T[this->dataSize];
            copyElem(a.data, this->len);
        }

        size_t len = 0;
        size_t dataSize = 0;
        T *data = nullptr;

        void resize(size_t newSize) {
            T *newArr = new T[newSize];
            for (size_t i = 0; i < len; ++i) {
                newArr[i] = data[i];
            }
            delete[] data;
            data = newArr;
            dataSize = newSize;
        }

    public:
        explicit ArrayList(size_t capacity) {
            dataSize = capacity;
            data = new T[dataSize];
            len = 0;
        }

        ArrayList() : ArrayList(15) {}

        ArrayList(const ArrayList<T> &a) {
            copy(a);
        }

        ~ArrayList() {
            delete[] data;
        }

        size_t length() const override {
            return len;
        }

        bool isEmpty() const override {
            return len == 0;
        }

        bool contains(const T &a) const override {
            for (int i = 0; i < len; ++i) {
                if (data[i] == a) return true;
            }
            return false;
        }

        class Iter : bczhc::Iterator<T> {
        private:
            size_t length;
            size_t i = 0;
            T *data;
        public:
            bool hasNext() override {
                return i < length;
            }

            T next() override {
                return this->data[i++];
            }

            Iter(T *data, size_t len) : data(data), length(len) {}
        };

        /*bczhc::Iterator<T> iterator() const override {
            return Iter(data, len);
        }*/

        void add(const T &a) override {
            if (len >= dataSize/* length + 1 > dataSize */)
                resize(2 * dataSize);
            data[len++] = a;
        }

        void clear() override {
            resize(15);
            len = 0;
        }

        T get(size_t index) const override {
            if (index < 0 || index >= len) throw bczhc::IndexOutOfBoundsException();
            return data[index];
        }

        void set(size_t index, const T &a) const override {
            if (index < 0 || index >= len) throw bczhc::IndexOutOfBoundsException();
            data[index] = a;
        }

        void add(size_t index, const T &a) override {
            if (index < 0 || index > len) throw bczhc::IndexOutOfBoundsException();
            if (len >= dataSize/* length + 1 > dataSize */)
                resize(dataSize * 2);
            for (size_t i = len; i > index; --i)
                data[i] = data[i - 1];
            data[index] = a;
            ++len;
        }

        T remove(size_t index) override {
            if (index < 0 || index >= len) throw bczhc::IndexOutOfBoundsException();
            T removed = data[index];
            --len;
            for (size_t i = index; i < len; ++i)
                data[i] = data[i + 1];
            if (len < dataSize / 4)
                resize(dataSize / 2);
            return removed;
        }

        ssize_t indexOf(const T &a) const override {
            for (size_t i = 0; i < len; ++i) {
                if (data[i] == a) return i;
            }
            return -1;
        }

        Iter getIterator() {
            return Iter(data, len);
        }

        void addAll(size_t index, const T *a, size_t size) {
            if (index < 0 || index > len) throw bczhc::IndexOutOfBoundsException();
            ssize_t t;
            size_t newSize = len + size;
            if (newSize > dataSize)
                resize(newSize);
            for (ssize_t i = len - 1 + size; i > (ssize_t) index; --i) {
                t = i - size;
                if (t >= 0) data[i] = data[t];
            }
            for (ssize_t i = index; i < (ssize_t) (index + size); ++i) {
                data[i] = a[i - index];
            }
            len += size;
        }

        void addAll(const T *a, size_t size) {
            size_t newSize = len + size;
            if (newSize > dataSize) {
                resize(2 * newSize);
            }
            for (size_t i = 0; i < size; ++i) {
                data[len + i] = a[i];
            }
            len += size;
        }

        /**
         * Remove range: [start, end)
         * @param start
         * @param end
         */
        void remove(size_t start, size_t end) {
            if (start < 0 || start >= len || end > len || end < start) return;
            size_t removedSize = end - start;
            size_t t = len - removedSize;
            for (size_t i = start; i < t; ++i) {
                data[i] = data[i + removedSize];
            }
            if (len < dataSize / 4) resize(dataSize / 2);
            len -= removedSize;
        }

        T &operator[](size_t i) const {
            return data[i];
        }

        ArrayList<T> &operator=(const ArrayList<T> &a) {
            if (&a == this) return *this;
            copy(a, true);
            return *this;
        }

        [[nodiscard]] size_t capacity() const {
            return dataSize;
        }

        [[nodiscard]] T *&getData() {
            return data;
        }
    };
}

#endif //BCZHC_CPP_ARRAY_LIST_HPP