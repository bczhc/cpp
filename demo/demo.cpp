#include "../concurrent.h"
#include "../file.h"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.h"
#include "../array.hpp"
#include "../def.h"
#include <cassert>
#include "../app/base128/Base128Lib.h"
#include "../sqlite3.hpp"
#include "../third_party/json-single-header/single_include/nlohmann/json.hpp"

extern "C" {
#include "../third_party/crypto-algorithms/sha256.h"
}

using namespace std;
using namespace bczhc;
using JSON = nlohmann::json;

int main() {
    const char *jsonPath = "/home/zhc/messages.json";
    const char *outputPath = "/home/zhc/online_class_questions_11_1.db";

    Sqlite3 db(outputPath);
    db.exec("BEGIN TRANSACTION");
    db.exec("DROP TABLE IF EXISTS question");
    db.exec("CREATE TABLE IF NOT EXISTS question (\n    subject_name TEXT NOT NULL,\n    question_owner_name TEXT NOT NULL,\n    question_owner_id INTEGER,\n    question_message TEXT NOT NULL,\n    second_timestamp INTEGER\n)");
    auto stat = db.compileStatement("INSERT INTO question VALUES(?,?,?,?,?)");


    InputStream is(jsonPath);
    String read;
    int readSize;
    char buf[4096];
    while ((readSize = is.read(buf, 4096)) > 0) {
        read.append(buf, readSize);
    }

    JSON json = JSON::parse(read.getCString());
    unsigned long size = json.size();
    for (int i = 0; i < size; ++i) {
        auto jsonObject = json[i];
        auto subjectName = jsonObject["subjectName"].get<string>();
        auto courses = jsonObject["courses"];
        auto courseSize = courses.size();
        for (int j = 0; j < courseSize; ++j) {
            auto course = courses[j];
            auto questions = course["questions"];
            auto qSize = questions.size();
            if (qSize == 0) continue;
            for (int k = 0; k < qSize; ++k) {
                auto question = questions[k];
                auto secondTimestamp = question["secondTimestamp"].get<int>();
                auto questionText = question["question"].get<string>();
                auto questionOwnerId = Integer::parseInt(question["questionOwnerId"].get<string>().c_str());
                auto questionOwner = question["questionOwner"].get<string>();
                stat.reset();
                stat.bindText(1, subjectName.c_str());
                stat.bindText(2, questionOwner.c_str());
                stat.bind(3, questionOwnerId);
                stat.bindText(4, questionText.c_str());
                stat.bind(5, secondTimestamp);
                stat.step();
            }
        }
    }
    stat.release();
    db.exec("COMMIT");
    db.close();
    cout << "done" << endl;
    return 0;
}