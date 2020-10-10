#include <iostream>
#include <functional>
#include <string>
#include <sys/time.h>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"
#include "../String.h"
#include "../Concurrent.h"
#include "thread"
#include "../Sqlite3.h"


using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

int main() {
    char o[] = "hello";
    String s = String::fromRef(o);
    cout << s.getCString() << endl;
    cout << (s.getCString() == o) << endl;

    auto splited = s.split("el");
    int size = splited.length();
    for (int i = 0; i < size; ++i) {
        cout << splited.get(i).getCString() << endl;
    }
    return 0;
}
