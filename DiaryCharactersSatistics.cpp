#include <iostream>
#include "CountCharacters.h"
#include "zhc.h"
#include <sqlite3.h>

using namespace std;

int callback(void *arg, int colNum, char **colVal, char **colName) {
	countCharacters(colVal[0], -1, (sqlite3 *) arg);
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
	sqlite3_exec(diaryDatabase, "SELECT content FROM diary", callback, (void *) resultDatabase, nullptr);
	return 0;
}
