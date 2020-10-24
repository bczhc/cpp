int main() {
    return 0;
}

#include <iostream>
using namespace std;

extern "C" int f() {
    string s = "fsadA";
    return s.length();
}