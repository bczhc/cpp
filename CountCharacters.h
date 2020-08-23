//
// Created by root on 2020/8/23.
//

#ifndef CPP_COUNTCHARACTERS_H
#define CPP_COUNTCHARACTERS_H

#include <sqlite3.h>

int countCharacters(const char *u8Bytes, int size, sqlite3 *resultDatabase);

#endif //CPP_COUNTCHARACTERS_H
