//
// Created by root on 2020/4/11.
//

#include <iostream>
#include "../zhc.h"

using namespace std;

int main() {
    char *s = nullptr;
    strcpyAndCat_auto(&s, "a", -1, "b", -1);
    cout << s << endl;
    delete s;
    return 0;
}