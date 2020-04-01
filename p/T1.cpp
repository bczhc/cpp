//
// Created by root on 2020/3/27.
//

#include "../zhc.h"

class A {
public:
    int a{};
};

void f(A &r) {
    r.a = 2;
}

int main() {
    A a;
    f(a);
    cout << a.a << endl;
    return 0;
}
