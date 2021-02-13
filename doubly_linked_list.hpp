//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_DOUBLY_LINKED_LISG_HPP
#define BCZHC_CPP_DOUBLY_LINKED_LISG_HPP

#include "./iterator.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class DoublyLinkedList {
    private:
        class Node {
        public:
            T data;
            Node *prev = nullptr;
            Node *next = nullptr;

            Node() = default;
        };

        Node *head, *last;
        int len = 0;
        int *refCount;

        void copy(const DoublyLinkedList<T> &a) {
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
        DoublyLinkedList() {
            head = new Node();
            last = head;
            refCount = new int(0);
        }

        ~DoublyLinkedList() {
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
            head->next = nullptr;
            last = head;
        }

        bool isEmpty() { return len == 0; }

        int length() { return len; }

        T get(int index) {
            Node *t = head;
            for (int i = 0; i <= index; ++i) {
                t = t->next;
            }
            return t->data;
        }

        void insert(T a) {
            Node *newNode = new Node();
            newNode->prev = last, newNode->data = a;
            last->next = newNode;
            last = last->next;
            ++len;
        }

        void insert(int index, T a) {
            Node *prev = head;
            for (int i = 0; i < index; ++i)
                prev = prev->next;
            Node *curr = prev->next;
            Node *newNode = new Node;
            newNode->prev = prev, newNode->data = a, newNode->next = curr;
            curr->prev = newNode;
            prev->next = newNode;
            ++len;
        }

        T remove(int index) {
            Node *prev = head;
            for (int i = 0; i < index; ++i)
                prev = prev->next;
            Node *curr = prev->next;
            T r = curr->data;
            Node *next = curr->next;
            if (next == nullptr) {
                last = curr;
            } else {
                prev->next = next;
                next->prev = prev;
            }
            delete curr;
            --len;
            return r;
        }

        int indexOf(T e) {
            Node *t = head;
            for (int i = 0; i < len; ++i) {
                t = t->next;
                if (e == t->data)
                    return i;
            }
        }

        T getFirst() { return head->next->data; }

        T getLast() { return last->data; }

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

        DoublyLinkedList(const DoublyLinkedList<T> &a) {
            copy(a);
            ++*refCount;
        }

        DoublyLinkedList<T> &operator=(const DoublyLinkedList<T> &a) {
            if (&a == this) return *this;
            release();
            copy(a);
            ++*refCount;
            return *this;
        }
    };
}

#endif //BCZHC_CPP_DOUBLY_LINKED_LISG_HPP
