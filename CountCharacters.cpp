#include <cstring>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by root on 2020/8/23.
//

#include "CountCharacters.h"
#include <sqlite3.h>
#include <iostream>
#include "zhc.h"

//must be larger than 12
#define BUFFER_SIZE 30

using namespace std;

struct SolvedProperties {
	int bytesLength;
	int codepoint;
};

inline int getUTF8BytesLength(const char firstByte) {
	if (firstByte >> 7 == 0) return 1;
	if (firstByte >> 5 == 0b110) return 2;
	if (firstByte >> 4 == 0b1110) return 3;
	if (firstByte >> 3 == 0b11110) return 4;
	if (firstByte >> 2 == 0b111110) return 5;
	if (firstByte >> 1 == 0b1111110) return 6;
	return 0;
}

inline void solveUTF8Bytes(SolvedProperties &solvedProperties, const char *bytes) {
	solvedProperties.bytesLength = getUTF8BytesLength(bytes[0]);
	switch(solvedProperties.bytesLength) {
		case 1:
			solvedProperties.codepoint = bytes[0] & 0b01111111;
			break;
		case 2:
			solvedProperties.codepoint = (bytes[1] & 0b00111111) | ((bytes[0] & 0b00011111) << 6);
			break;
		case 3:
			solvedProperties.codepoint =
				(bytes[2] & 0b00111111) | ((bytes[1] & 0b00111111) << 6) | ((bytes[0] & 0b00001111) << 12);
			break;
		case 4:
			solvedProperties.codepoint =
				(bytes[3] & 0b00111111) | ((bytes[2] & 0b00111111) << 6) | ((bytes[1] & 0b00001111) << 12) |
				((bytes[0] & 0b00000111) << 18);
			break;
		case 5:
			solvedProperties.codepoint =
				(bytes[4] & 0b00111111) | ((bytes[3] & 0b00111111) << 6) | ((bytes[2] & 0b00001111) << 12) |
				((bytes[1] & 0b00000111) << 18) | ((bytes[0] & 0b00000011) << 24);
			break;
		case 6:
			solvedProperties.codepoint =
				(bytes[5] & 0b00111111) | ((bytes[4] & 0b00111111) << 6) | ((bytes[3] & 0b00001111) << 12) |
				((bytes[2] & 0b00000111) << 18) | ((bytes[1] & 0b00000011) << 24) | ((bytes[0] & 0b00000001) << 30);
			break;
	}
}

int checkIfExistCallback(void *arg, int colNum, char **content, char **colName) {
	*(int *) arg = 1;
	return 1;
}

inline int checkIfRowExist(sqlite3 *database, const char *cmd) {
	int result = 0;
	sqlite3_exec(database, cmd, checkIfExistCallback, &result, nullptr);
	return result;
}

int countCharacters(const char *u8Bytes, int size, sqlite3 *resultDatabase) {
	char *errMsg = nullptr;
	FILE *file = nullptr;
	sqlite3_exec(resultDatabase, "CREATE TABLE IF NOT EXISTS chars (\n    codepoint integer,\n    count integer\n);",
			nullptr, nullptr, &errMsg);
	sqlite3_exec(resultDatabase, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
	if (size == -1) size = strlen(u8Bytes);
	int offset = 0;
	SolvedProperties solvedProperties{};
	while (offset < size) {
		solveUTF8Bytes(solvedProperties, u8Bytes + offset);
		offset += solvedProperties.bytesLength;
		int codepoint = solvedProperties.codepoint;
		// ::cout << (char) codepoint;
		char *cmd = nullptr;
		char *numStr = nullptr;
		m_itoa(&numStr, codepoint);
		strcpyAndCat_auto(&cmd, "SELECT codepoint FROM chars WHERE codepoint is ", -1, numStr, -1);
		if (!checkIfRowExist(resultDatabase, cmd)) {
			char *cmd2 = nullptr;
			strcpyAndCat_auto(&cmd2, "INSERT INTO chars VALUES(", -1, numStr, -1);
			char *oldStrPoint = cmd2;
			strcpyAndCat_auto(&cmd2, cmd2, -1, ",0);", -1); 
			delete oldStrPoint;
			sqlite3_exec(resultDatabase, cmd2, nullptr, nullptr, &errMsg);
			delete cmd2;
		}
		delete cmd;
		char *cmd3 = nullptr;
		strcpyAndCat_auto(&cmd3, "UPDATE chars SET count=count+1 where codepoint is ", -1, numStr, -1);
		sqlite3_exec(resultDatabase, cmd3, nullptr, nullptr, &errMsg);
		delete cmd3;
	}
	sqlite3_exec(resultDatabase, "COMMIT;", nullptr, nullptr, &errMsg);
	return 0;
}




#pragma clang diagnostic pop
