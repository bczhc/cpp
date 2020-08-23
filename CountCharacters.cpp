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

inline void solveUTF8Bytes(SolvedProperties &solvedProperties, const unsigned char *bytes) {
    unsigned char firstByte = bytes[0];
    if (firstByte >> 7 == 0) {
        solvedProperties.bytesLength = 1;
        solvedProperties.codepoint = firstByte & 0b01111111;
    } else if (firstByte >> 5 == 0b110) {
        solvedProperties.bytesLength = 2;
        solvedProperties.codepoint = (bytes[1] & 0b00111111) | ((bytes[0] & 0b00011111) << 6);
    } else if (firstByte >> 4 == 0b1110) {
        solvedProperties.bytesLength = 3;
        solvedProperties.codepoint =
                (bytes[2] & 0b00111111) | ((bytes[1] & 0b00111111) << 6) | ((bytes[0] & 0b00001111) << 12);
    } else if (firstByte >> 3 == 0b11110) {
        solvedProperties.bytesLength = 4;
        solvedProperties.codepoint =
                (bytes[3] & 0b00111111) | ((bytes[2] & 0b00111111) << 6) | ((bytes[1] & 0b00001111) << 12) |
                ((bytes[0] & 0b00000111) << 18);
    } else if (firstByte >> 2 == 0b111110) {
        solvedProperties.bytesLength = 5;
        solvedProperties.codepoint =
                (bytes[4] & 0b00111111) | ((bytes[3] & 0b00111111) << 6) | ((bytes[2] & 0b00001111) << 12) |
                ((bytes[1] & 0b00000111) << 18) | ((bytes[0] & 0b00000011) << 24);
    } else if (firstByte >> 1 == 0b1111110) {
        solvedProperties.bytesLength = 6;
        solvedProperties.codepoint =
                (bytes[5] & 0b00111111) | ((bytes[4] & 0b00111111) << 6) | ((bytes[3] & 0b00001111) << 12) |
                ((bytes[2] & 0b00000111) << 18) | ((bytes[1] & 0b00000011) << 24) | ((bytes[0] & 0b00000001) << 30);
    } else solvedProperties.bytesLength = 0, solvedProperties.codepoint = 0;
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

int main() {
    char *errMsg = nullptr;
    FILE *file = nullptr;
    if ((file = fopen("/home/zhc/code/code/Android/some-tools/app/src/main/java/pers/zhc/tools/jni/JNI.java", "rb"))
        == nullptr) {
        ::cout << "open file error" << ::endl;
        return 1;
    }
    sqlite3 *resultDatabase;
    if (sqlite3_open("./countResult.db", &resultDatabase)) {
        ::cout << "open or create database error" << ::endl;
        return 1;
    }
    sqlite3_exec(resultDatabase, "CREATE TABLE IF NOT EXISTS chars (\n    codepoint integer,\n    count integer\n);",
                 nullptr, nullptr, &errMsg);
    SolvedProperties solvedProperties{};
    unsigned char buffer[BUFFER_SIZE];
    int readOffset = 0;
    int firstByteOffset = 0;
    int readLen;
    while ((readLen = fread(buffer + readOffset, 1, BUFFER_SIZE - readOffset, file)) != 0) {
        for (int j = 0; j < BUFFER_SIZE; ++j) {
            //not enough for last utf8 bytes whose length at most is 6,
            //so we copy the rest bytes to the buffer holding by next read.
            int restBytesLength = BUFFER_SIZE - firstByteOffset;
            if (restBytesLength < 6) {
                for (int k = firstByteOffset; k < BUFFER_SIZE; ++k) {
                    buffer[k - firstByteOffset] = buffer[k];
                }
                readOffset = restBytesLength;
                firstByteOffset = 0;
                break;
            }
            solveUTF8Bytes(solvedProperties, buffer + firstByteOffset);
            ::cout << (char) solvedProperties.codepoint << "";
            firstByteOffset += solvedProperties.bytesLength;
        }
    }
    return 0;
}

#pragma clang diagnostic pop