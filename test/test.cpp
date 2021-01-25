#include "../concurrent.h"
#include "../file.h"
#include "../io.h"
#include "../math/fourier_series.h"
#include "../regex.h"
#include "../sqlite3.h"
#include "../string.hpp"
#include "../third_party/practice/LinearList.hpp"
#include "../utils.hpp"
#include "thread"
#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <ifaddrs.h>
#include <iostream>
#include <netinet/in.h>
#include <new>
#include <pcre.h>
#include <pthread.h>
#include <string>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;

MutexLock lock;         // NOLINT(cert-err58-cpp)
CountDownLatch latch(2);// NOLINT(cert-err58-cpp)

template<typename T>
class Array {
private:
    int len = 0;
    int *deleteCount = nullptr;
public:
    T *elements;

    explicit Array(int size) {
        elements = new T[size];
        len = size;
        deleteCount = new int(0);
    }

    Array(const Array<T> &arr) {
        this->len = arr.len;
        this->elements = arr.elements;
        this->deleteCount = arr.deleteCount;
        ++(*deleteCount);
    }

    int length() {
        return len;
    }

    ~Array() {
        if (deleteCount == nullptr) return;
        if (--*deleteCount == -1) {
            delete[] elements;
            delete deleteCount;
        }
    }

    Array<T> &operator=(const Array<T> &arr) {
        if (&arr == this) return *this;

        this->len = arr.len;
        this->elements = arr.elements;
        this->deleteCount = arr.deleteCount;
        ++(*deleteCount);
        return *this;
    }
};

void f(Array<int> a) {
    for (int i = 0; i < a.length(); ++i) {
        cout << a.elements[i] << endl;
    }
}

int main(int argc, char **argv) {
    Array<int> arr(5);
    for (int i = 0; i < 5; ++i) {
        arr.elements[i] = i;
    }

    for (int i = 0; i < 5; ++i) {
        cout << arr.elements[i] << endl;
    }

    return 0;
}
