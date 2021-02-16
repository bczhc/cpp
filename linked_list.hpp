//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_LINKED_LIST_HPP
#define BCZHC_CPP_LINKED_LIST_HPP

#include "./iterator.hpp"
#include "./exception.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class LinkedList {
    private:
        class Node {
        public:
            T data{};
            Node *next = nullptr;

            Node(T data, Node *next) : data(data), next(next) {}

            Node() = default;
        };

        struct Properties {
            Node *head = nullptr;
            size_t length = 0;
            int refCount = 0;
        };

        Properties *prop = nullptr;

        void release() {
            if (--prop->refCount == -1) {
                clear();
                delete prop->head;
                delete prop;
            }
        }

    public:
        LinkedList() {
            prop = new Properties;
            prop->head = new Node;
        }

        ~LinkedList() {
            release();
        }

        void clear() {
            prop->length = 0;
            Node *t = prop->head->next, *prev;
            while (t != nullptr) {
                prev = t;
                t = t->next;
                delete prev;
            }
        }

        bool isEmpty() { return prop->length == 0; }

        int length() { return prop->length; }

        T get(int index) {
            if (index < 0 || index >= prop->length) throw IndexOutOfBoundsException();
            Node *t = prop->head;
            for (int i = 0; i <= index; ++i) {
                t = t->next;
            }
            return t->data;
        }

        void insert(T a) {
            Node *t = prop->head;
            while (t->next != nullptr)
                t = t->next;
            Node *newNode = new Node(a, nullptr);
            t->next = newNode;
            ++prop->length;
        }

        void insert(int index, T a) {
            if (index < 0 || index > prop->length) throw IndexOutOfBoundsException();
            Node *t = prop->head;
            for (int i = 0; i < index; ++i) {
                t = t->next;
            }
            Node *newNode = new Node(a, t->next);
            t->next = newNode;
            ++prop->length;
        }

        T remove(int index) {
            if (index < 0 || index >= prop->length) throw IndexOutOfBoundsException();
            Node *t = prop->head;
            for (int i = 0; i < index; ++i) {
                t = t->next;
            }
            Node *removedNode = t->next;
            t->next = removedNode->next;
            T r = removedNode->data;
            delete removedNode;
            --prop->length;
            return r;
        }

        T getFirst() {
            if (length() < 1) throw NoSuchElementException();
            return prop->head->next->data;
        }

        void insertFirst(T a) {
            auto newNode = new Node(a, prop->head->next);
            prop->head->next = newNode;
            ++prop->length;
        }

        T removeFirst() {
            auto firstNode = prop->head->next;
            prop->head->next = firstNode->next;
            T t = firstNode->data;
            delete firstNode;
            --prop->length;
            return t;
        }

        int indexOf(T e) {
            Node *t = prop->head;
            for (int i = 0; i < prop->length; ++i) {
                t = t->next;
                if (t->data == e)
                    return i;
            }
            return -1;
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
            Iterator it(prop->head);
            return it;
        }

    private:
        Node *reverse(Node *node) {
            if (node->next == nullptr) {
                prop->head->next = node;
                return node;
            }
            Node *prev = reverse(node->next);
            prev->next = node;
            return node;
        }

    public:
        void reverse() {
            if (isEmpty())
                return;
            reverse(prop->head->next)->next = nullptr;
        }

        T getMid() {
            Node *slow = prop->head->next, *fast = slow;
            while (fast != nullptr && fast->next != nullptr)
                slow = slow->next, fast = fast->next->next;
            return slow->data;
        }

        LinkedList(const LinkedList<T> &a) {
            prop = a.prop;
            ++prop->refCount;
        }

        LinkedList<T> &operator=(const LinkedList<T> &a) {
            if (&a == this) return *this;
            release();
            prop = a.prop;
            ++prop->refCount;
            return *this;
        }
    };
}

#endif //BCZHC_CPP_LINKED_LIST_HPP
