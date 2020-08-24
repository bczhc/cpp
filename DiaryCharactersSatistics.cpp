#include <iostream>
#include "CountCharacters.h"
#include "zhc.h"
#include <map>
#include <sqlite3.h>
#include "utf8.h"

using namespace std;
using namespace bczhc::utf8;

CharacterCounter *counter;

int callback(void *arg, int colNum, char **colVal, char **colName) {
	counter->countCharacters(colVal[0], -1);
	return 0;
}

int main() {
	sqlite3 *resultDatabase = nullptr;
	if (sqlite3_open("./countResult.db", &resultDatabase)) {
		::cout << "Open or create result database error." << ::endl;
		return 1;
	}
	sqlite3_exec(resultDatabase, "CREATE TABLE IF NOT EXISTS chars(codepoint INTEGER, count INTEGER);", nullptr, nullptr, nullptr);
	sqlite3 *diaryDatabase = nullptr;
	if (sqlite3_open("/storage/emulated/0/diary.db", &diaryDatabase)) {
		::cout << "Open diary database failed." << ::endl;
		return 1;
	}
	counter = new CharacterCounter;
	sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, nullptr, nullptr);
	map<int, int64_t>::iterator it = counter->data->begin();
	char u8Char[5];
	for (; it != counter->data->end(); it++) {
		int size = getUTF8Size(it->first);
		unicode2UTF8(u8Char, it->first), u8Char[size] = '\0';
		::cout << it->first << "|" << it->second << ::endl;	
	}
	return 0;
}
