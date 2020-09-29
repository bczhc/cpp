#include "../String.h"
#include "../third_party/practice/LinearList.hpp"

using namespace bczhc;
using namespace string;
using namespace std;
using namespace linearlist;

void f() {
    String e = "exception here";
    throw e;
}

int main() {
    try {
        f();
    } catch (String e) {
        cout << e.getCString() << endl;
    }
    return 0;
}
