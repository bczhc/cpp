//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_ITERATOR_HPP
#define BCZHC_CPP_ITERATOR_HPP

namespace bczhc {
    template<typename T>
    class Iterator {
    public:
        virtual bool hasNext() = 0;

        virtual T next() = 0;
    };
}

#endif //BCZHC_CPP_ITERATOR_HPP
