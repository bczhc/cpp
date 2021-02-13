//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_SYMBOL_TABLE_HPP
#define BCZHC_CPP_SYMBOL_TABLE_HPP

#include "./exception.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename K, typename V>
    class SymbolTable {
    public:
        class Node {
        public:
            K key;
            V value;
            Node *next = nullptr;

            Node(K key, V value) : key(key), value(value) {}

            Node() = default;
        };

    private:
        Node *head = nullptr;
        int len = 0;
        int *refCount;

        void copy(const SymbolTable<K, V> &a) {
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
        SymbolTable() {
            refCount = new int(0);
            head = new Node;
        }

        ~SymbolTable() {
            release();
        }

        V get(K key) {
            Node *t = head;
            for (int i = 0; i < len; ++i) {
                t = t->next;
                if (t->key == key) {
                    return t->value;
                }
            }
            throw KeyNotFoundException();
        }

        virtual void put(K key, V value) {
            Node *t = head;
            for (int i = 0; i < len; ++i) {
                t = t->next;
                if (t->key == key) {
                    t->value = value;
                    return;
                }
            }
            Node *newNode = new Node(key, value);
            Node *first = head->next;
            newNode->next = first;
            head->next = newNode;
            ++len;
        }

        void remove(K key) {
            Node *t = head;
            for (int i = 1; i < len; ++i) {
                if (t->next->key == key) {
                    Node *removedNode = t->next;
                    t->next = removedNode->next;
                    delete removedNode;
                }
            }
            --len;
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

        int size() { return len; }

        struct Bean {
            K key;
            V val;

            Bean(K key, V val) : key(key), val(val) {}
        };

        class Iterator : bczhc::Iterator<Bean> {
        private:
            Node *t;
        public:
            explicit Iterator(Node *head) : t(head) {}

            bool hasNext() override {
                return t->next != nullptr;
            }

            Bean next() override {
                t = t->next;
                return Bean(t->key, t->value);
            }
        };

        Iterator getIterator() {
            return Iterator(head);
        }

        SymbolTable(const SymbolTable<K, V> &a) {
            copy(a);
            ++*refCount;
        }

        SymbolTable<K, V> &operator=(const SymbolTable<K, V> &a) {
            if (&a == this) return *this;
            release();
            copy(a);
            ++*refCount;
            return *this;
        }
    };
} // namespace bczhc

#endif //BCZHC_CPP_SYMBOL_TABLE_HPP
