#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include <sys/time.h>


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
