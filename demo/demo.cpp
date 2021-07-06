#include "../concurrent.hpp"
#include "../file.hpp"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.hpp"
#include "../array.hpp"
#include "../def.hpp"
#include <cassert>
#include "../app/base128/Base128Lib.h"
#include "../sqlite3.hpp"
#include <memory>
#include "../array_list.hpp"
#include "../linked_list.hpp"
#include "../stack.hpp"
#include "../queue.hpp"
#include "../iterator.hpp"
#include "../symbol_table.hpp"
#include "../shared_pointer.hpp"
#include <unordered_map>
#include "../regex.hpp"
#include "../io.hpp"
#include <random>
#include <termios.h>
#include <cstdarg>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../app/charactersStatistics/CountCharacters.h"

using namespace std;
using namespace bczhc;

int main(int argc, char **argv) {
    Sqlite3 db("/home/bczhc/private-repo/diary/private/diary.db");
    Sqlite3::Statement statement = db.compileStatement("SELECT \"date\", content FROM diary;");
    Sqlite3::Cursor cursor = statement.getCursor();

    struct Bean {
        int32_t date;
        size_t len;
        size_t unique_len;
        double use_rate;
    };

    vector<Bean> vec;

    while (cursor.step()) {
        CharacterCounter counter;
        int32_t date = cursor.getInt(0);
        String content(cursor.getText(1));
        counter.countCharacters(content.getCString(), (int) content.length());
        unsigned long unique_len = counter.data->size();
        vec.push_back(
                Bean{
                        date,
                        content.utf8Length(),
                        unique_len,
                        static_cast<double>(unique_len) / static_cast<double>(content.utf8Length())
                }
        );
    }

    statement.release();
    db.close();

    auto comparer = [](const Bean &o1, const Bean &o2) {
        double d = o1.use_rate - o2.use_rate;
        return d < 0;
    };

    sort(vec.begin(), vec.end(), comparer);

    for (const auto &item : vec) {
        cout << item.date << ' ' << item.unique_len << " : " << item.len << " = "
             << (static_cast<double>(item.unique_len) /
                 static_cast<double>(item.len) * 100)
             << '%' << endl;
    }

    return 0;
}