//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_LINKED_LIST_HPP
#define BCZHC_CPP_LINKED_LIST_HPP

#include "./iterator.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename T>
    class LinkedList {
    private:
        class Node {
        public:
            T data;
            Node *next = nullptr;

            Node(T data, Node *next) : data(data), next(next) {}

            Node() = default;
        };

        Node *head;
        int len = 0;
        int *refCount;

        void copy(const LinkedList<T> &a) {
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
        LinkedList() {
            refCount = new int(0);
            head = new Node();
        }

        ~LinkedList() {
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

        int length() { return len; }

        T get(int index) {
            Node *t = head;
            for (int i = 0; i <= index; ++i) {
                t = t->next;
            }
            return t->data;
        }

        void insert(T a) {
            Node *t = head;
            while (t->next != nullptr)
                t = t->next;
            Node *newNode = new Node(a, nullptr);
            t->next = newNode;
            ++len;
        }

        void insert(int index, T a) {
            Node *t = head;
            for (int i = 0; i < index; ++i) {
                t = t->next;
            }
            Node *newNode = new Node(a, t->next);
            t->next = newNode;
            ++len;
        }

        T remove(int index) {
            Node *t = head;
            for (int i = 0; i < index; ++i) {
                t = t->next;
            }
            Node *removedNode = t->next;
            t->next = removedNode->next;
            T r = removedNode->data;
            delete removedNode;
            --len;
            return r;
        }

        int indexOf(T e) {
            Node *t = head;
            for (int i = 0; i < len; ++i) {
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
            Iterator it(head);
            return it;
        }

    private:
        Node *reverse(Node *node) {
            if (node->next == nullptr) {
                head->next = node;
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
            reverse(head->next)->next = nullptr;
        }

        T getMid() {
            Node *slow = head->next, *fast = slow;
            while (fast != nullptr && fast->next != nullptr)
                slow = slow->next, fast = fast->next->next;
            return slow->data;
        }

        LinkedList(const LinkedList<T> &a) {
            copy(a);
            ++*refCount;
        }

        LinkedList<T> &operator=(const LinkedList<T> &a) {
            if (&a == this) return *this;
            release();
            copy(a);
            ++*refCount;
            return *this;
        }
    };
}

#endif //BCZHC_CPP_LINKED_LIST_HPP
