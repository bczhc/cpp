#include "../concurrent.h"
#include "../file.h"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.h"
#include "../array.hpp"
#include "../def.h"
#include <cassert>
#include "../app/base128/Base128Lib.h"
#include "../sqlite3.hpp"
#include <memory>
#include "../array_list.hpp"
#include "../linked_list.hpp"
#include "../stack.hpp"
#include "../queue.hpp"
#include "../iterator.hpp"
#include "../symbol_table.hpp"

using namespace std;
using namespace bczhc;

template<typename T>
class SharedPointer {
private:
    T *ptr;
    int *count{};

    void release() const {
        if (--*count == -1) {
            delete ptr;
            delete count;
        }
    }

public:
    SharedPointer(T *ptr) : ptr(ptr) { // NOLINT(google-explicit-constructor)
        count = new int(0);
    }

    T *get() {
        return this->ptr;
    }

    SharedPointer(const SharedPointer &a) {
        this->count = a.count;
        this->ptr = a.ptr;
        ++*count;
    }

    SharedPointer &operator=(const SharedPointer &a) {
        if (&a == this) return *this;
        release();
        this->ptr = a.ptr;
        this->count = a.count;
        ++*count;
        return *this;
    }

    ~SharedPointer() {
        release();
    }

    T *operator->() {
        return this->ptr;
    }

    T *operator*() {
        return this->ptr;
    }
};

template<typename T>
using SP = SharedPointer<T>;

void f2(SP<String> s) {
    cout << s->getCString() << endl;
}

void f(SP<String> s) {
    auto s2 = s;
    auto s3 = s;
    auto s4 = s2, s5 = s3;
    f2(s2), f2(s4);
    s5 = s2;
    s5 = s;
    s5 = s3;
    s = s4;
    s = s3;
}

int main() {
    SymbolTable<String, int> t;
    t.put("a", 2);
    auto r = t.get("a");
    cout << r << endl;
    return 0;
}