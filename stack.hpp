//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_STACK_HPP
#define BCZHC_CPP_STACK_HPP

#include "./iterator.hpp"

using namespace bczhc;

template<typename T>
class Stack {
private:
    using Node = typename LinkedList<T>::Node;
    Node *head = nullptr;
    int len = 0;
    int *refCount;

    void copy(const Stack<T> &a) {
        this->head = a.head;
        this->len = a.len;
        this->refCount = a.refCount;
    }

    void release() {
        if (--*refCount == -1) {
            clear();
            delete head;
            delete refCount;
        }
    }

public:
    Stack() {
        refCount = new int(0);
        head = new Node();
    }

    ~Stack() {
        release();
    }

    void clear() {
        len = 0;
        Node *t = head->next, *prev;
        while (t != nullptr) {
            prev = t;
            t = t->next;
            delete prev;
        }
    }

    bool isEmpty() { return len == 0; }

    int size() { return len; }

    T pop() {
        Node *first = head->next;
        T r = first->data;
        head->next = first->next;
        delete first;
        --len;
        return r;
    }

    void push(T a) {
        Node *newNode = new Node(a, head->next);
        head->next = newNode;
        ++len;
    }

    class Iterator : public bczhc::Iterator<T> {
    private:
        Node *t;

    public:
        explicit Iterator(Node *head) : t(head) {}

        bool hasNext() override {
            return t->next != nullptr;
        }

        T next() override {
            t = t->next;
            return t->data;
        }
    };

    Iterator getIterator() {
        Iterator it(head);
        return it;
    }

    Stack(const Stack<T> &a) {
        copy(a);
        ++*refCount;
    }

    Stack<T> &operator=(const Stack<T> &a) {
        if (&a == this) return *this;
        release();
        copy(a);
        ++*refCount;
        return *this;
    }
};

#endif //BCZHC_CPP_STACK_HPP
