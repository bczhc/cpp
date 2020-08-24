#include <iostream>
#include "CountCharacters.h"
#include "zhc.h"
#include <map>
#include <sqlite3.h>

using namespace std;

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
	sqlite3 *diaryDatabase = nullptr;
	if (sqlite3_open("/storage/emulated/0/diary.db", &diaryDatabase)) {
		::cout << "Open diary database failed." << ::endl;
		return 1;
	}
	counter = new CharacterCounter;
	sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, nullptr, nullptr);
	map<int, int64_t>::iterator it = counter->data->begin();
	for (; it != counter->data->end(); it++) {
		::cout << it->first << "|" << it->second << ::endl;
	}
	return 0;
}
