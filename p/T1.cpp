//
// Created by root on 2020/3/27.
//
#include <iostream>

using namespace std;

template<typename T>
class Node {
public:
    T data;
    Node *next;
};

template<typename T>
class ArrayList {
private:
    Node<T> *head;
    int32_t lastIndex;

    Node<T> *getNode(int index) {
        Node<T> * p = head;
        for (int32_t i = 0; i < index; ++i) {
            p = p->next;
        }
        return p;
    }

public:
    ArrayList() {
        head = nullptr;
        lastIndex = 0;
    }

    ArrayList &add(T a) {
        if (head == nullptr) {
            head = new Node<T>;
            head->next = nullptr;
            head->data = a;
        } else {
            Node<T> *p = head;
            for (int32_t i = 0; i < lastIndex; ++i) {
                p = p->next;
            }
            p->next = new Node<T>;
            p->next->data = a;
            ++lastIndex;
        }
        return *this;
    }

    T get(int32_t index) {
        Node<T> *p = head;
        for (int i = 0; i < index; ++i) {
            p = p->next;
        }
        return p->data;
    }

    void release() {
        for (int32_t i = 0; i < lastIndex; ++i) {
            delete getNode(i);
        }
    }

    int32_t length() {
        return this->lastIndex + 1;
    }
};

int main() {
    ArrayList<int> list;
    list.add(2)
            .add(3)
            .add(4);
    int32_t l = list.length();
    for (int i = 0; i < l; ++i) {
        cout << list.get(i) << endl;
    }
    list.release();
    return 0;
}
