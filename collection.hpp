//
// Created by bczhc on 3/6/21.
//

#ifndef BCZHC_CPP_COLLECTION_HPP
#define BCZHC_CPP_COLLECTION_HPP

#include "iterator.hpp"

namespace bczhc {
    template<typename T>
    class Collection {
    public:
        virtual size_t length() const = 0;

        virtual bool isEmpty() const = 0;

        virtual bool contains(const T &a) const = 0;

//        virtual bczhc::Iterator<T> iterator() const = 0;

        virtual void add(const T &a) = 0;

        virtual void clear() = 0;
    };
}

#endif //BCZHC_CPP_COLLECTION_HPP
