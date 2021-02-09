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
#include "../third_party/json-single-header/single_include/nlohmann/json.hpp"

extern "C" {
#include "../third_party/crypto-algorithms/sha256.h"
}

using namespace std;
using namespace bczhc;
using JSON = nlohmann::json;

class A {
public:
    A() {
        cout << "constructing..." << endl;
    }

    ~A() {
        cout << "destructing..." << endl;
    }

    A(const A &a) {
        cout << "copying..." << endl;
    }
};

A f(A a) {
    return a;
}

int main() {
    A a;

    String s;
    cout << ("" == s) << endl;
    cout << ("" != s) << endl;
    return 0;
}
