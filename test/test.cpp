#include "../String.h"

using namespace bczhc;
using namespace string;
using namespace std;

int main() {
    String s = "hello there";
    String s2 = "hi this";
    String s3 = (s = s2);
    cout << s.getCString() << endl;
    cout << s2.getCString() << endl;
    cout << s3.getCString() << endl;
    return 0;
}
