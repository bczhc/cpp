#include "../Concurrent.h"
#include "../FourierSeries.h"
#include "../Sqlite3.h"
#include "../String.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include "../utils.h"
#include "../File.h"
#include <map>
#include <sys/wait.h>

using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace std;

MutexLock lock; // NOLINT(cert-err58-cpp)

int main() {
    cout << "begin transaction;" << endl;
    for (char c = 'a'; c <= 'z'; ++c) {
        String sql = "create table wubi_code_";
        sql.append(c)
        .append("(code text not null primary key, word text not null);");
        cout << sql.getCString() << endl;
    }
    struct WordBean {
        std::string code;
        String word;
        int64_t num{};

        WordBean(std::string code, String word, int64_t num) : code(code), word(word), num(num) {}

        WordBean() {}
    };
    using LList = LinkedList<WordBean>;
    using Map = map<std::string, LList *>;
    auto *m = new Map();

    Sqlite3 db;
    db.open("/root/some-tools/app/src/main/res/raw/wubi_dict.db");
    class CB : public Sqlite3::SqliteCallback {
    private:
        Map &m;
    public:
        explicit CB(Map &m) : m(m) {}

    public:
        int callback(void *arg, int colNum, char **content, char **colName) override {
            int64_t num = atoll(content[2]);
            std::string code = content[0];
            WordBean bean(code, content[1], num);
            if (m.find(code) == m.end()) {
                auto *newList = new LList();
                newList->insert(bean);
                m[code] = newList;
            } else {
                m.find(code)->second->insert(bean);
            }
            return 0;
        }
    } cb(*m);

    db.exec("select code,char,num from wubi_dict order by num desc", cb);

    auto iter = m->begin();
    for (;iter != m->end(); ++iter) {
        auto candidateList = iter->second;
        std::string code = candidateList->get(0).code;
        int candidatesLength = candidateList->length();
        WordBean beans[candidatesLength];
        int i = 0;
        auto listIter = candidateList->getIterator();
        if (listIter.moveToFirst()) {
            do {
                auto wordBean = listIter.get();
                beans[i++] = wordBean;
            } while (listIter.next());
        }

        class C : public sort::Comparable<WordBean> {
            public:
            int compare(WordBean &o1, WordBean &o2) override {
                return o2.num - o1.num;
            }
        } comparable;


        String candidateCombinedString = beans[0].word;
        for (int j = 1; j < i; ++j) {
            candidateCombinedString.append('|').append(beans[j].word);
        }
        char theFirstCodeChar = code.c_str()[0];
        String sql = "insert into wubi_code_";
        sql.append(theFirstCodeChar)
        .append(" values(")
        .append('\'')
        .append(candidateList->get(0).code)
        .append("','")
        .append(candidateCombinedString)
        .append("');");
        cout << sql.getCString() << endl;
        delete candidateList;
    }
    delete m;
    cout << "commit;" << endl;
    return 0;
}
