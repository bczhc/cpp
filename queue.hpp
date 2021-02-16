//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_QUEUE_HPP
#define BCZHC_CPP_QUEUE_HPP

#include "./iterator.hpp"
#include "./doubly_linked_list.hpp"

template<typename T>
class Queue : public DoublyLinkedList<T> {
public:
    T dequeue() {
        return this->removeLast();
    }

    void enqueue(T a) {
        this->insertFirst(a);
    }
};

#endif //BCZHC_CPP_QUEUE_HPP
