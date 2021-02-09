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
    const char *outputPath = "/home/zhc/online_class_chat_messages_11_1.db";

    Sqlite3 db(outputPath);
    db.exec("BEGIN TRANSACTION");
    db.exec("DROP TABLE IF EXISTS message");
    db.exec("CREATE TABLE IF NOT EXISTS message\n(\n    subject_name     TEXT NOT NULL,\n    sender_id        INTEGER,\n    sender_name      TEXT NOT NULL,\n    text             TEXT NOT NULL,\n    rich_text        TEXT NOT NULL,\n    second_timestamp INTEGER\n)");
    auto stat = db.compileStatement("INSERT INTO message VALUES(?,?,?,?,?,?)");

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
            auto chatMessages = course["chatMessages"];
            auto charMessagesSize = chatMessages.size();
            for (int k = 0; k < charMessagesSize; ++k) {
                auto chat = chatMessages[k];
                auto senderId = Integer::parseInt(chat["senderId"].get<string>().c_str());
                auto sender = chat["sender"].get<string>();
                auto text = chat["text"].get<string>();
                auto richText = chat["richText"].get<string>();
                auto timestamp = chat["timestamp"].get<int>();

                stat.reset();
                stat.bindText(1, subjectName.c_str());
                stat.bind(2, senderId);
                stat.bindText(3, sender.c_str());
                stat.bindText(4, text.c_str());
                stat.bindText(5, richText.c_str());
                stat.bind(6, timestamp);
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