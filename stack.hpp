//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_STACK_HPP
#define BCZHC_CPP_STACK_HPP

#include "./iterator.hpp"
#include "linked_list.hpp"

using namespace bczhc;

template<typename T>
class Stack : LinkedList<T> {
public:
    void push(T a) {
        this->insertFirst(a);
    }

    T pop() {
        return this->removeFirst();
    }

    T peek() {
        return this->getFirst();
    }
};

#endif //BCZHC_CPP_STACK_HPP
