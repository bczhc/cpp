#include "../string.hpp"
#include <iostream>
#include "../io.h"
#include <sys/time.h>

using namespace std;
using namespace bczhc;

#define RANDOM(a, b) (((double) rand() / RAND_MAX) * ((b) - (a)) + (a))

int main() {
    String str;
    InputStream is(stdin);
    char buf[2048];
    int read;
    while ((read = is.read(buf, 2048)) > 0) {
        str.append(buf, read);
    }
    is.close();
    const char *s = str.getCString();

    timeval t{};
    gettimeofday(&t, nullptr);
    srand(t.tv_sec * 1000 + t.tv_usec / 1000);
    using List = LinkedList<int>;
    List list;
    SolvedUTF8Properties solved{};
    int i = 0;
    while (s[i] != '\0') {
        solveUTF8Bytes(solved, (uchar *) (s + i));
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
