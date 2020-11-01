#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "../utf8.h"
#include <iostream>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace utf8;
using namespace string;

#define RANDOM(a, b) (((double) rand() / RAND_MAX) * ((b) - (a)) + (a))

int main() {
    const char *s = "wo xian zai fei chang gao xing ren shi ni.";

    srand(time(0));
    using List = LinkedList<int>;
    List list;
    SolvedUTF8Properties solved;
    int i = 0;
    while (s[i] != '\0') {
        solveUTF8Bytes(solved, s + i);
        i += solved.bytesLength;
        list.insert(solved.codepoint);
    }

    char u8Bytes[5];
    String result;
    while (list.length() != 0) {
        int ran = RANDOM(0, list.length() - 1);
        u8Bytes[unicode2UTF8(u8Bytes, list.remove(ran))] = '\0';
        result.append(u8Bytes);
    }

    cout << result.getCString() << endl;
    return 0;
}
