//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_QUEUE_HPP
#define BCZHC_CPP_QUEUE_HPP

#include "./iterator.hpp"

template<typename T>
class Queue {
private:
    class Node {
    public:
        T data;
        Node *next = nullptr;

        Node(T data, Node *next) : data(data), next(next) {}

        Node() = default;
    };

    Node *head = nullptr, *last = nullptr;
    int len = 0;
    int *refCount;

    void copy(const Queue<T> &a) {
        this->head = a.head;
        this->last = a.last;
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
    Queue() {
        head = new Node;
        last = head;
        refCount = new int(0);
    }

    ~Queue() {
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

    T dequeue() {
        Node *first = head->next;
        if (first->next == nullptr) {
            last = head;
        }
        head->next = first->next;
        T r = first->data;
        delete first;
        --len;
        return r;
    }

    void enqueue(T a) {
        last->next = new Node(a, nullptr);
        last = last->next;
        ++len;
    }

    class Iterator : public bczhc::Iterator<T> {
    private:
        Node *t = nullptr;

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

    Queue(const Queue<T> &a) {
        copy(a);
        ++*refCount;
    }

    Queue<T> &operator=(const Queue<T> &a) {
        if (&a == this) return *this;
        release();
        copy(a);
        ++*refCount;
        return *this;
    }
};

#endif //BCZHC_CPP_QUEUE_HPP
