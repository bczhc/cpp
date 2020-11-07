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

#include "../third_party/json-single-header/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

int main() {
    json j = json::parse("{\"a\": 7}");
    cout << j["a"] << endl;
    return 0;
}
