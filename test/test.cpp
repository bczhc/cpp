#include "../String.h"
#include "../third_party/practice/LinearList.hpp"

using namespace bczhc;
using namespace string;
using namespace std;
using namespace linearlist;

int main(int argc, char **argv) {
    if (argc < 3) return 0;
    String s = argv[1];
    SequentialList<String> list = s.split(argv[2]);
    int len = list.length();
    for (int i = 0; i < len; ++i) {
        cout << list.get(i).getCString() << endl;
    }
    return 0;
}
