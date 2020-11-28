#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <functional>
#include <iostream>
#include <string>
#include "../utils.h"
#include "../File.h"
#include <map>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;

MutexLock lock; // NOLINT(cert-err58-cpp)

int convert2Int(const String &s) {
    int r = 0;
    int len = (int) s.size();
    for (int i = len - 1; i >= 0; --i) {
        r += (s.getCString()[i] - 'a' + 1) * (int) pow(26, len - 1 - i);
    }
    return r;
}

int main() {
    using List = SequentialList<String *>;
    char *marks = new char[100000];
    memset(marks, 0, 100000);
    List list;

    Sqlite3 db;
    db.open("/home/zhc/code/some-tools/app/src/main/res/raw/wubi_dict.db");
    class CB : public Sqlite3::SqliteCallback {
    private:
        char *marks;
        List &list;
    public:
        explicit CB(char *marks, List &list) : marks(marks), list(list) {}

    public:
        int callback(void *arg, int colNum, char **content, char **colName) override {
            auto *code = new String(content[0]);
            int mark = convert2Int(*code);
            if (marks[mark] == 0) {
                list.insert(code);
                marks[mark] = 1;
            }
            return 0;
        }
    } cb(marks, list);
    db.exec("select code from wubi_dict order by num desc", cb);
    return 0;
}
