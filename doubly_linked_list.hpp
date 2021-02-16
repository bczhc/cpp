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

        struct Properties {
            Node *head = nullptr, *tail = nullptr;
            size_t length = 0;
            int refCount = 0;
        };

        Properties *prop = nullptr;

        /**
         * Get node, beginning from the first data head (the next of the dummy head).
         * @param index index
         * @return node
         */
        Node *getNode(size_t index) const {
            auto t = prop->head;
            for (size_t i = 0; i <= index; ++i) {
                t = t->next;
            }
            return t;
        }

        void initDummy() {// dummy node
            prop->head = new Node(nullptr, nullptr);
            // dummy node
            prop->tail = new Node(nullptr, nullptr);
            prop->head->next = prop->tail, prop->tail->prev = prop->head;
        }

        void release() {
            if (--prop->refCount == -1) {
                clear();
                delete prop->head, delete prop->tail;
                delete prop;
            }
        }

    public:
        DoublyLinkedList() {
            prop = new Properties;
            initDummy();
        }

        ~DoublyLinkedList() {
            release();
        }

        void clear() {
            prop->length = 0;
            Node *t = prop->head->next, *prev;
            while (t != prop->tail) {
                prev = t;
                t = t->next;
                delete prev;
            }
            prop->head->next = prop->tail, prop->tail->prev = prop->head;
        }

        bool isEmpty() const { return prop->length == 0; }

        int length() const { return prop->length; }

        T get(size_t index) const {
            if (index < 0 || index >= prop->length) throw IndexOutOfBoundsException();
            return getNode(index)->data;
        }

        void insert(T a) {
            auto newNode = new Node(prop->tail->prev, a, prop->tail);
            auto prev = prop->tail->prev;
            prev->next = newNode;
            prop->tail->prev = newNode;
            ++prop->length;
        }

        void insertLast(T a) {
            insert(a);
        }

        void insertFirst(T a) {
            auto newNode = new Node(prop->head, a, prop->head->next);
            auto next = prop->head->next;
            prop->head->next = newNode, next->prev = newNode;
            ++prop->length;
        }

        void insert(size_t index, T a) {
            if (index < 0 || index > prop->length) throw IndexOutOfBoundsException();
            if (index == 0) insertFirst(a);
            else if (index == prop->length) insertLast(a);
            else {
                auto prev = getNode(index - 1), *next = prev->next;
                auto newNode = new Node(prev, a, next);
                prev->next = newNode, next->prev = newNode;
                ++prop->length;
            }
        }

        void insertAll(const DoublyLinkedList<T> &a) {
            auto it = a.getIterator();
            while (it.hasNext()) {
                insert(it.next());
            }
        }

        T remove(size_t index) {
            if (index < 0 || index >= prop->length) throw IndexOutOfBoundsException();
            auto removed = getNode(index);
            T r = removed->data;
            removed->prev->next = removed->next;
            removed->next->prev = removed->prev;
            delete removed;
            return r;
        }

        ssize_t indexOf(T e) const {
            auto t = prop->head->next;
            for (size_t i = 0; t != prop->tail; ++i) {
                if (t->data == e) return i;
                t = t->next;
            }
            return -1;
        }

        T removeFirst() {
            auto firstNode = prop->head->next;
            prop->head->next = firstNode->next;
            firstNode->next->prev = prop->head;
            --prop->length;
            T t = firstNode->data;
            delete firstNode;
            return t;
        }

        T removeLast() {
            auto lastNode = prop->tail->prev;
            lastNode->prev->next = prop->tail;
            prop->tail->prev = lastNode->prev;
            T t = lastNode->data;
            delete lastNode;
            return t;
        }

        T getFirst() const {
            if (prop->length < 1) throw NoSuchElementException();
            return prop->head->next->data;
        }

        T getLast() const {
            if (prop->length < 1) throw NoSuchElementException();
            return prop->tail->prev->data;
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
            Iterator it(prop->head, prop->tail);
            return it;
        }

        DoublyLinkedList(const DoublyLinkedList<T> &a) {
            prop = a.prop;
            ++prop->refCount;
        }

        DoublyLinkedList<T> &operator=(const DoublyLinkedList<T> &a) {
            if (&a == this) return *this;
            release();
            prop = a.prop;
            ++prop->refCount;
            return *this;
        }
    };
}

#endif //BCZHC_CPP_DOUBLY_LINKED_LISG_HPP
