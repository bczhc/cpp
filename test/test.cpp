#include <iostream>
#include <functional>
#include <sys/time.h>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"
#include "../String.h"
#include "../Concurrent.h"
#include "../Sqlite3.h"
#include "../CountCharacters.h"
#include <csignal>
#include "thread"
#include "../Sqlite3.h"

#include "../zhc.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

String int2bin(int32_t i) {
    SequentialList<bool> list;
    int x = i;
    while (x != 0) {
        list.insert(0, x % 2);
        x /= 2;
    }
    int len = list.length();
    char chars[len + 1];
    chars[len] = '\0';
    for (int j = 0; j < len; ++j) {
        chars[j] = (char) ((int) list.get(j) + 48);
    }
    return chars;
}

int main() {
    int i = 0x12345678;
    int *p = &i;
    char i1 = *((char *) p + 0);
    char i2 = *((char *) p + 1);
    char i3 = *((char *) p + 2);
    char i4 = *((char *) p + 3);
    cout << hex << (int) i1 << endl;
    cout << hex << (int) i2 << endl;
    cout << hex << (int) i3 << endl;
    cout << hex << (int) i4 << endl;
    return 0;
}