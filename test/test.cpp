//
// Created by bczhc on 2/7/21.
//

#include "test.h"
#include "../string.hpp"
#include <iostream>

using namespace bczhc::string;
using namespace std;

int main() {
    int status = 1;
    try {
        status = test();
    } catch (const String &s) {
        cout << s.getCString() << endl;
    }
    return status;
}
