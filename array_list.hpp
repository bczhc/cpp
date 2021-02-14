//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_ARRAY_LIST_HPP
#define BCZHC_CPP_ARRAY_LIST_HPP

namespace bczhc {
    template<typename T>
    class ArrayList {
    private:
        int len = 0;
        int dataSize = 0;
        int *refCount;

        void resize(int newSize) {
            T *newArr = new T[newSize];
            for (int i = 0; i < len; ++i)
                newArr[i] = data[i];
            delete[] data;
            data = newArr;
            dataSize = newSize;
        }

        void release() {
            if (--*refCount == -1) {
                delete[] data;
                delete refCount;
            }
        }

        void copy(const ArrayList<T> &a) {
            this->data = a.data;
            this->dataSize = a.dataSize;
            this->refCount = a.refCount;
            this->len = a.len;
        }

    public:
        T *data = nullptr;

        explicit ArrayList(int capacity) {
            refCount = new int(0);
            data = new T[capacity];
            dataSize = capacity;
        }

        ArrayList() {
            refCount = new int(0);
            data = new T[1];
            dataSize = 1;
        }

        ~ArrayList() { release(); }

        void clear() { len = 0; }

        bool isEmpty() { return len == 0; }

        int length() { return len; }

        T get(int i) { return data[i]; }

        void insert(int index, T a) {
            if (len == dataSize)
                resize(dataSize * 2 + 2);
            for (int i = len; i > index; --i)
                data[i] = data[i - 1];
            data[index] = a;
            ++len;
        }

        void insert(int index, const T *a, int size) {
            if (index < 0 || index > len) return;
            int t;
            if (len + size >= dataSize)
                resize(dataSize * 2 + len + size);
            for (int i = len - 1 + size; i > index; --i) {
                t = i - size;
                if (t >= 0) data[i] = data[t];
            }
            for (int i = index; i < index + size; ++i) {
                data[i] = a[i - index];
            }
            len += size;
        }

        ArrayList<T> &insert(T a) {
            if (len == dataSize)
                resize(dataSize * 2 + 2);
            data[len++] = a;
            return *this;
        }

        T remove(int index) {
            if (len < dataSize / 4)
                resize(dataSize / 2);
            T removed = data[index];
            --len;
            for (int i = index; i < len; ++i)
                data[i] = data[i + 1];
            return removed;
        }

        /**
         * Remove range: [start, end)
         * @param start
         * @param end
         */
        void remove(int start, int end) {
            if (start < 0 || start >= len || end > len || end < start) return;
            int removedSize = end - start;
            int t = len - removedSize;
            for (int i = start; i < t; ++i) {
                data[i] = data[i + removedSize];
            }
            if (len < dataSize / 4) resize(dataSize / 2);
            len -= removedSize;
        }

        int indexOf(T t) {
            for (int i = 0; i < len; ++i) {
                if (data[i] == t)
                    return i;
            }
        }

        T &operator[](int i) {
            return data[i];
        }

        ArrayList(const ArrayList<T> &a) {
            copy(a);
            ++*refCount;
        }

        ArrayList<T> &operator=(const ArrayList<T> &a) {
            if (&a == this) return *this;
            release();
            copy(a);
            ++*refCount;
            return *this;
        }
    };
}

#endif //BCZHC_CPP_ARRAY_LIST_HPP