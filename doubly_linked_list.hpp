//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_DOUBLY_LINKED_LISG_HPP
#define BCZHC_CPP_DOUBLY_LINKED_LISG_HPP

#include "./iterator.hpp"
#include "./exception.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class DoublyLinkedList {
    private:
        class Node {
        public:
            Node *prev = nullptr;
            T data{};
            Node *next = nullptr;

            Node() = default;

            Node(Node *prev, T data, Node *next) : prev(prev), data(data), next(next) {}

            Node(Node *prev, Node *next) : prev(prev), next(next) {}
        };

        Node *head = nullptr, *last = nullptr;
        size_t len = 0;

        /**
         * Get node, beginning from the first data head (the next of the dummy head).
         * @param index index
         * @return node
         */
        Node *getNode(size_t index) const {
            auto t = head;
            for (size_t i = 0; i <= index; ++i) {
                t = t->next;
            }
            return t;
        }

        void initDummy() {// dummy node
            head = new Node(nullptr, nullptr);
            // dummy node
            last = new Node(nullptr, nullptr);
            head->next = last, last->prev = head;
        }

    public:
        DoublyLinkedList() {
            initDummy();
        }

        ~DoublyLinkedList() {
            clear();
            delete head, delete last;
        }

        void clear() {
            len = 0;
            Node *t = head->next, *prev;
            while (t != last) {
                prev = t;
                t = t->next;
                delete prev;
            }
            head->next = last, last->prev = head;
        }

        bool isEmpty() const { return len == 0; }

        int length() const { return len; }

        T get(size_t index) const {
            if (index < 0 || index >= len) throw IndexOutOfBoundsException();
            return getNode(index)->data;
        }

        void insert(T a) {
            auto newNode = new Node(last->prev, a, last);
            auto prev = last->prev;
            prev->next = newNode;
            last->prev = newNode;
            ++len;
        }

        void insertLast(T a) {
            insert(a);
        }

        void insertFirst(T a) {
            auto newNode = new Node(head, a, head->next);
            head->next = newNode, head->next->prev = newNode;
            ++len;
        }

        void insert(size_t index, T a) {
            if (index < 0 || index > len) throw IndexOutOfBoundsException();
            if (index == 0) insertFirst(a);
            else if (index == len) insertLast(a);
            else {
                auto prev = getNode(index - 1), *next = prev->next;
                auto newNode = new Node(prev, a, next);
                prev->next = newNode, next->prev = newNode;
                ++len;
            }
        }

        void insertAll(const DoublyLinkedList<T> &a) {
            auto it = a.getIterator();
            while (it.hasNext()) {
                insert(it.next());
            }
        }

        T remove(size_t index) {
            if (index < 0 || index >= len) throw IndexOutOfBoundsException();
            auto removed = getNode(index);
            T r = removed->data;
            removed->prev->next = removed->next;
            removed->next->prev = removed->prev;
            delete removed;
            return r;
        }

        ssize_t indexOf(T e) const {
            auto t = head->next;
            for (size_t i = 0; t != last; ++i) {
                if (t->data == e) return i;
                t = t->next;
            }
            return -1;
        }

        T getFirst() const {
            if (len < 1) throw NoSuchElementException();
            return head->next->data;
        }

        T getLast() const {
            if (len < 1) throw NoSuchElementException();
            return last->prev->data;
        }

        class Iterator : public bczhc::Iterator<T> {
        private:
            Node *t;
            Node *last;

        public:
            explicit Iterator(Node *head, Node *last) : t(head), last(last) {}

            bool hasNext() override {
                return t->next != last;
            }

            T next() override {
                t = t->next;
                return t->data;
            }
        };

        Iterator getIterator() const {
            Iterator it(head, last);
            return it;
        }

        DoublyLinkedList(const DoublyLinkedList<T> &a) {
            initDummy();
            insertAll(a);
        }

        DoublyLinkedList<T> &operator=(const DoublyLinkedList<T> &a) {
            if (&a == this) return *this;
            clear();
            insertAll(a);
            return *this;
        }
    };
}

#endif //BCZHC_CPP_DOUBLY_LINKED_LISG_HPP
