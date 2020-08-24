#include "zhc.h"
#include <cstdarg>
#include <vector>

using namespace bczhc;
using namespace std;

int main() {
    struct A {
        int a;
    };
    A a;
    a.a = 1;
    vector<A> vec;
    vec.push_back(a);
    cout << vec[0].a << endl;
    a.a = 2;
    cout << vec[0].a << endl;
    return 0;
}
