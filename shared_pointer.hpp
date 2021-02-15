//
// Created by bczhc on 2/14/21.
//

#ifndef BCZHC_CPP_SHARED_POINTER_HPP
#define BCZHC_CPP_SHARED_POINTER_HPP

namespace bczhc {
    template<typename T>
    class SharedPointer {
    private:
        T *ptr;
        int *refCount{};

        void release() const {
            if (--*refCount == -1) {
                delete ptr;
                delete refCount;
            }
        }

    public:
        SharedPointer(T *ptr) : ptr(ptr) { // NOLINT(google-explicit-constructor)
            refCount = new int(0);
        }

        T *get() {
            return this->ptr;
        }

        SharedPointer(const SharedPointer &a) {
            this->refCount = a.refCount;
            this->ptr = a.ptr;
            ++*refCount;
        }

        SharedPointer &operator=(const SharedPointer &a) {
            if (&a == this) return *this;
            release();
            this->ptr = a.ptr;
            this->refCount = a.refCount;
            ++*refCount;
            return *this;
        }

        ~SharedPointer() {
            release();
        }

        T *operator->() {
            return this->ptr;
        }

        T &operator*() {
            return *this->ptr;
        }
    };

    template <typename T>
    using SP = SharedPointer<T>;
}

#endif //BCZHC_CPP_SHARED_POINTER_HPP
