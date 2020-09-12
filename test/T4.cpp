//
// Created by root on 2020/4/11.
//

#include <iostream>
#include <unistd.h>
#include "../zhc.h"

using namespace std;

class A {
public:
    A() {
        cout << "constructing A" << endl;
    }

    ~A() {
        cout << "destructing A" << endl;
    }
};

int main() {
    return 0;
}