//
// Created by bczhc on 3/6/21.
//

#ifndef BCZHC_CPP_LIST_H
#define BCZHC_CPP_LIST_H

namespace bczhc {
    template<typename T>
    class List : public Collection<T> {
    public:
        virtual T get(size_t index) const = 0;

        virtual void set(size_t index, const T &a) const = 0;

        virtual void add(size_t index, const T &a) = 0;

        virtual T remove(size_t index) = 0;

        virtual ssize_t indexOf(const T &a) const = 0;
    };
}

#endif //BCZHC_CPP_LIST_H
