//
// Created by root on 2020/3/27.
//

#include "T2.h"
#include "../zhc.h"

int main() {
    char s[] = " ";
    char **dest = nullptr;
    int i = Split(&dest, s, -1, "", -1);
    for (int j = 0; j < i; ++j) {
        cout << dest[j] << endl;
    }
    string s2 = "aa";
    char str[s2.size() + 1];
    str[s2.size()] = 0;
    s2.copy(str, s2.size(), 0);
    cout << str << endl;
    return 0;
}