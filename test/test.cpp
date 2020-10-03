#include <iostream>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"
#include "../String.h"
#include "../Concurrent.h"
#include "../Sqlite3.h"
#include "../CountCharacters.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;

MutexLock lock;

int main() {
    CharacterCounter counter;
    for (int i = 0; i < 1000; ++i) {
        String s = "abc";
        cout << s.append(" code: ").append(to_string(21)).getCString() << endl;
        counter.countCharacters(s.getCString(), strlen(s.getCString()));
    }
    json *j = counter.getJsonData();
    cout << j->dump() << endl;
    delete j;
    return 0;
}