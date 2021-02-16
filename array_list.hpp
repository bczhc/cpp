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
    private:
        void copy(const ArrayList<T> &a) {
            this->prop = a.prop;
        }

    protected:
        struct Properties {
            size_t length = 0;
            size_t dataSize = 0;
            int refCount = 0;
            T *data = nullptr;
        };

        Properties *prop = nullptr;

        void release() {
            if (--prop->refCount == -1) {
                delete[] prop->data;
                delete prop;
            }
        }

        void resize(size_t newSize) {
            T *newArr = new T[newSize];
            for (size_t i = 0; i < prop->length; ++i)
                newArr[i] = prop->data[i];
            delete[] prop->data;
            prop->data = newArr;
            prop->dataSize = newSize;
        }

    public:
        explicit ArrayList(size_t capacity) {
            prop = new Properties;
            prop->data = new T[capacity];
            prop->dataSize = capacity;
            prop->length = 0;
        }

        ArrayList() : ArrayList(15) {}

        ArrayList(const ArrayList<T> &a) {
            copy(a);
            ++prop->refCount;
        }

        ~ArrayList() {
            release();
        }

        virtual void clear() {
            resize(15);
            prop->length = 0;
        }

        [[nodiscard]] bool isEmpty() const { return prop->length == 0; }

        [[nodiscard]] int length() const { return prop->length; }

        T get(int i) const { return prop->data[i]; }

        virtual void insert(size_t index, T a) {
            if (prop->length >= prop->dataSize/* length + 1 > dataSize */)
                resize(prop->dataSize * 2);
            for (size_t i = prop->length; i > index; --i)
                prop->data[i] = prop->data[i - 1];
            prop->data[index] = a;
            ++prop->length;
        }

        virtual void insert(size_t index, const T *a, size_t size) {
            if (index < 0 || index > prop->length) return;
            ssize_t t;
            size_t newSize = prop->length + size;
            if (newSize > prop->dataSize)
                resize(newSize);
            for (ssize_t i = prop->length - 1 + size; i > (ssize_t) index; --i) {
                t = i - size;
                if (t >= 0) prop->data[i] = prop->data[t];
            }
            for (ssize_t i = index; i < (ssize_t) (index + size); ++i) {
                prop->data[i] = a[i - index];
            }
            prop->length += size;
        }

        virtual void insert(T a) {
            if (prop->length >= prop->dataSize/* length + 1 > dataSize */)
                resize(2 * prop->dataSize);
            prop->data[prop->length++] = a;
        }

        virtual void insert(const T *a, size_t size) {
            size_t newSize = prop->length + size;
            if (newSize > prop->dataSize) {
                resize(2 * newSize);
            }
            for (size_t i = 0; i < size; ++i) {
                prop->data[prop->length + i] = a[i];
            }
            prop->length += size;
        }

        virtual T remove(int index) {
            if (prop->length < prop->dataSize / 4)
                resize(prop->dataSize / 2);
            T removed = prop->data[index];
            --prop->length;
            for (size_t i = index; i < prop->length; ++i)
                prop->data[i] = prop->data[i + 1];
            return removed;
        }

        /**
         * Remove range: [start, end)
         * @param start
         * @param end
         */
        virtual void remove(size_t start, size_t end) {
            if (start < 0 || start >= prop->length || end > prop->length || end < start) return;
            size_t removedSize = end - start;
            size_t t = prop->length - removedSize;
            for (size_t i = start; i < t; ++i) {
                prop->data[i] = prop->data[i + removedSize];
            }
            if (prop->length < prop->dataSize / 4) resize(prop->dataSize / 2);
            prop->length -= removedSize;
        }

        ssize_t indexOf(T t) const {
            for (size_t i = 0; i < prop->length; ++i) {
                if (prop->data[i] == t)
                    return i;
            }
            return -1;
        }

        T &operator[](size_t i) const {
            return prop->data[i];
        }

        ArrayList<T> &operator=(const ArrayList<T> &a) {
            if (&a == this) return *this;
            release();
            copy(a);
            ++prop->refCount;
            return *this;
        }

        [[nodiscard]] size_t capacity() const {
            return prop->dataSize;
        }

        [[nodiscard]] T *&getData() {
            return prop->data;
        }
    };
}

#endif //BCZHC_CPP_ARRAY_LIST_HPP